#include "EntityPlayer.h"
#include <HAPI_lib.h>
#include <algorithm>
#include "Rectangle.h"
#include "SoundManager.h"

//Deadzone for analogoue sticks on 360 controller
#define INPUT_DEADZONE  7849

EntityPlayer::EntityPlayer(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side) : Entity(textureMapName, health, damage, posX, posY, side)
{
	isPlayer = true;
	m_type = eEntityType::eTypePlayer;
	m_sounds.jump = "Jump";
}

void EntityPlayer::Update(Simulation& simulation)
{

	float currentTime = static_cast <float>  (HAPI.GetTime());

	checkWinCondition(simulation);

	UpdatePhysics();

	UserInput(simulation);

	switch (m_CurrentState)
	{

	case State::eStand:

		m_textureMapName = "playerIdle";

		m_Acceleration.x = 0;
		m_Acceleration.y = 0;

		//All possible ranges of movement from the standing state.

		//aka falling
		if (!m_OnGround)
		{
			m_CurrentState = State::eJumping;
			break;
		}

		//Left/Right Movement Key Pressed
		if (KeyState(KeyInput::eGoRight) != KeyState(KeyInput::eGoLeft))
		{
			m_CurrentState = State::eWalk;
			break;
		}

		//Jump Key Pressed
		else if (KeyState(KeyInput::eJump))
		{
			SMM.PlaySound(m_sounds.jump);
			m_Velocity.y = -m_JumpSpeed;

			m_CurrentState = State::eJumping;
			break;
		}
		//Shoot key pressed
		else if (KeyState(KeyInput::eFire))
		{
			m_CurrentState = State::eShooting;
			break;
		}

		break;

	case State::eWalk:
		//if theres no tile to walk on fall.

		m_textureMapName = "playerRun";

		if (KeyState(KeyInput::eJump))
		{
			m_Velocity.y = m_JumpSpeed;
			m_CurrentState = State::eJumping;
			break;
		}

		//if both or neither left or right keys are pressed then stop.
		if (KeyState(KeyInput::eGoRight) == KeyState(KeyInput::eGoLeft))
		{
			m_CurrentState = State::eStand;

			m_Velocity.x = 0;
			m_Velocity.y = 0;

			break;
		}
		else if (KeyState(KeyInput::eGoRight))
		{
			m_Velocity.x = m_WalkSpeed;
		}
		else if (KeyState(KeyInput::eGoLeft))
		{
			m_Velocity.x = -m_WalkSpeed;
		}

		break;
	case State::eJumping:

		//Gravity
		m_Acceleration.y += 0.01f;
		//Max falling speed.
		m_Acceleration.y = std::max(m_Acceleration.y, 10.0f);

		if (!KeyState(KeyInput::eJump) && m_Acceleration.y > 0.0f)
		{
			m_Velocity.y = std::min(m_Acceleration.y, m_JumpSpeed);
		}

		if (KeyState(KeyInput::eGoRight) == KeyState(KeyInput::eGoLeft))
		{
			m_Velocity.x = 0.0f;
		}
		else if (KeyState(KeyInput::eGoRight))
		{
			m_Velocity.x = m_WalkSpeed;
		}
		else if (KeyState(KeyInput::eGoLeft))
		{
			m_Velocity.x = -m_WalkSpeed;
		}

		//if we hit the ground
		if (m_OnGround)
		{
			//if there's no movement change state to standing
			if (m_Inputs[(int)KeyInput::eGoRight] == m_Inputs[(int)KeyInput::eGoLeft])
			{
				m_CurrentState = State::eStand;
				m_Velocity.x = 0;
				m_Velocity.y = 0;
			}
			else    //either go right or go left are pressed so we change the state to walk
			{
				m_CurrentState = State::eWalk;
				m_Velocity.y = 0.0f;

			}
		}
		else
		{
			m_textureMapName = "playerJump";
		}

		break;
	case State::eShooting:

		m_textureMapName = "playerShoot";
		HAPI.SetControllerRumble(0, 65535, 65535);

		//Fire Delay
		if (currentTime > m_ShootingDelay + m_timeLastFired)
		{
			simulation.SpawnBullet(1, 0, m_side, getPosition().x, getPosition().y);
			m_timeLastFired = static_cast <float>  (HAPI.GetTime());
			HAPI.SetControllerRumble(0, 0, 0);
			m_CurrentState = State::eStand;
		}

		break;
	}


	if (getPosition().y > m_groundLevel)
	{
		setPosition(Vector2(getPosition().x, m_groundLevel));
		m_OnGround = true;
	}

	else
	{
		m_OnGround = false;
	}


}

void EntityPlayer::UserInput(Simulation& simulation)
{
	const HAPISPACE::HAPI_TControllerData& gamepadData = HAPI.GetControllerData(0);
	const HAPISPACE::HAPI_TKeyboardData& keyData = HAPI.GetKeyboardData();

	bool isGamePadEnabled = 1;
	float normalizedLX{ 0 };
	float normalizedLY{ 0 };

	if (gamepadData.isAttached)
	{

		float LX = static_cast <float> (gamepadData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_X]);
		float LY = static_cast <float> (gamepadData.analogueButtons[HK_ANALOGUE_LEFT_THUMB_Y]);

		//determine how far the controller is pushed
		float magnitude = sqrt(LX * LX + LY * LY);

		//determine the direction the controller is pushed
		normalizedLX = LX / magnitude;
		normalizedLY = LY / magnitude;

		float normalizedMagnitude = 0;

		//check if the controller is outside a circular dead zone
		if (magnitude > INPUT_DEADZONE)
		{
			//clip the magnitude at its expected maximum value
			if (magnitude > 32767) magnitude = 32767;

			//adjust magnitude relative to the end of the dead zone
			magnitude -= INPUT_DEADZONE;

			//optionally normalize the magnitude with respect to its expected range
			//giving a magnitude value of 0.0 to 1.0
			normalizedMagnitude = magnitude / (32767 - INPUT_DEADZONE);
		}

		else //if the controller is in the deadzone zero out the magnitude
		{
			magnitude = 0.0;
			normalizedMagnitude = 0.0;
		}

	}


	//Input Bindings
	//Each input changes a value on the m_inputs array to make the control system more self contained and easier to read.
	if (keyData.scanCode[HK_RETURN] || gamepadData.digitalButtons[HK_DIGITAL_A])
	{
		m_Inputs[(int)KeyInput::eFire] = true;

	}
	else
	{
		m_Inputs[(int)KeyInput::eFire] = false;
	}

	if (keyData.scanCode[HK_SPACE]
		|| keyData.scanCode['W']
		|| keyData.scanCode[HK_UP]
		|| gamepadData.isAttached && isGamePadEnabled && normalizedLY > 0.99
		|| gamepadData.digitalButtons[HK_DIGITAL_DPAD_UP])
	{

		m_Inputs[(int)KeyInput::eJump] = true;

	}
	else
	{
		m_Inputs[(int)KeyInput::eJump] = false;
	}
	if (keyData.scanCode['S'] || keyData.scanCode[HK_DOWN] || gamepadData.isAttached && isGamePadEnabled && normalizedLY < -0.99)
	{

	}
	if (keyData.scanCode['A'] || keyData.scanCode[HK_LEFT] || gamepadData.isAttached && isGamePadEnabled && normalizedLX < -0.99 || gamepadData.digitalButtons[HK_DIGITAL_DPAD_LEFT])
	{
		m_Inputs[(int)KeyInput::eGoLeft] = true;
	}
	else
	{
		m_Inputs[(int)KeyInput::eGoLeft] = false;
	}
	if (keyData.scanCode['D'] || keyData.scanCode[HK_RIGHT] || gamepadData.isAttached && isGamePadEnabled && normalizedLX > 0.99 || gamepadData.digitalButtons[HK_DIGITAL_DPAD_RIGHT])
	{
		m_Inputs[(int)KeyInput::eGoRight] = true;
	}
	else
	{
		m_Inputs[(int)KeyInput::eGoRight] = false;
	}

}

bool EntityPlayer::KeyState(const KeyInput& key)
{
	return (m_Inputs[(int)key]);
}

void EntityPlayer::setGrounded()
{
	m_OnGround = true;
}

void EntityPlayer::checkWinCondition(Simulation& simulation)
{
	if (getPosition().x >= simulation.getWorldEndZone())
	{
		simulation.setWin();
	}
}


