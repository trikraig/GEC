#include "Bipedal.h"
#include <HAPI_lib.h>

Bipedal::Bipedal(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side) : Entity(textureMapName, health, damage, posX, posY, side)
{
	m_sounds.death = "RobotDeath";
	m_sounds.enemyDiscovered = "RobotActivate";

	switch (side)
	{
	case 0:
		m_type = eEntityType::eTypeNeutral;
		break;
	case 1:
		m_type = eEntityType::eTypePlayer;
		break;
	case 2:
		m_type = eEntityType::eTypeEnemy;
		break;
	default:
		m_type = eEntityType::eTypeNeutral;
		break;
	}


}

void Bipedal::Update(Simulation& simulation)
{


	UpdatePhysics();

	float currentTime = static_cast <float>  (HAPI.GetTime());

	Vector2& playerPos = simulation.getPlayerPosition();

	switch (mCurrentState)
	{
	case State::eStand:
	{
		m_textureMapName = "bipedalIdle";

		m_Acceleration.x = 0;
		m_Acceleration.y = 0;

		//Returns to Idle/Stand if player dies.
		if (simulation.isPlayerAlive())
		{
			//Pursue left
			if (playerPos.x < getPosition().x)
			{
				m_WalkSpeed = -m_WalkSpeed;
				mCurrentState = State::eWalk;
			}
			//Pursue right
			else if (playerPos.x > getPosition().x)
			{
				m_WalkSpeed = m_WalkSpeed;
				mCurrentState = State::eWalk;
			}
		}

		break;
	}

	case State::eWalk:
	{
		m_textureMapName = "bipedalWalk";

		m_Velocity.x = m_WalkSpeed;

		if (playerPos.x <= (getPosition().x + m_ShootingRange) && simulation.isPlayerAlive())
		{
			//Player in range, begin shooting.
			m_Velocity.x = 0;
			mCurrentState = State::eShooting;
		}

		break;
	}


	case State::eShooting:
	{
		//No shooting animation for unit. Idle looks closest to what would be shooting
		m_textureMapName = "bipedalIdle";

		if (!simulation.isPlayerAlive())
		{
			mCurrentState = State::eStand;
		}

		//Fire Delay
		if (currentTime > m_ShootingInterval + m_timeLastFired)
		{
			//Player within range, shoot.
			simulation.SpawnBullet(1, 0, m_side, getPosition().x, getPosition().y);
			m_timeLastFired = static_cast <float>  (HAPI.GetTime());

		}

		//Check if player goes out of visible range.
		if (playerPos.x > (getPosition().x + m_ShootingRange))
		{
			//Player moved out of range, pursue.
			mCurrentState = State::eWalk;
		}

		break;
	}
	}
}
