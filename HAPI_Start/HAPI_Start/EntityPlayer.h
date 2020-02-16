#pragma once
#include "Entity.h"

//As more possible controls added, increase. e.g crouch
enum KeyInput
{
	eGoLeft, eGoRight, eGoDown, eJump, eFire, eIdle, Count
};

class EntityPlayer : public Entity
{
private:

	bool m_OnGround{ false };

	State m_CurrentState{ State::eStand };

	KeyInput m_CurrentKey{ eIdle };

	//Equal to total number of controllable inputs, increase with new options.
	bool m_Inputs[5]{ false };

	float m_JumpSpeed{ 50 };
	float m_WalkSpeed{ 5 };

	float m_ShootingDelay{ 1000 };
	float m_timeLastFired{ 0 };

	//Should be a set standard rather than dependant on screen size, maybe import from level later on.
	float m_groundLevel{ 600 };

public:

	EntityPlayer(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side);
	void Update(Simulation& simulation) override final;
	void UserInput(Simulation& simulation);
	bool KeyState(const KeyInput& key);
	void setGrounded() override final;
	void checkWinCondition(Simulation& simulation);

};


