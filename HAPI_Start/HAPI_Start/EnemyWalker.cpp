#include "EnemyWalker.h"

EnemyWalker::EnemyWalker(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side) : Entity(textureMapName, health, damage, posX, posY, side)
{
	m_type = eEntityType::eTypeEnemy;
	m_sounds.death = "AlienDeath";
}

void EnemyWalker::Update(Simulation& simulation)
{
	UpdatePhysics();

	//Aids calculating range in front and behind of entity.
	float negativeRange = getPosition().x - m_attackRange;
	float positiveRange = getPosition().x + m_attackRange;

	Vector2& playerPos = simulation.getPlayerPosition();

	switch (mCurrentState)
	{
	case State::eStand:
	{
		m_textureMapName = "alienIdle";

		m_Acceleration.x = 0;
		m_Acceleration.y = 0;

		if (simulation.isPlayerAlive())
		{
			//Player has to be within range to pursue.
			if (playerPos.x > negativeRange&& playerPos.x < getPosition().x)
			{
				m_WalkSpeed = -m_WalkSpeed;
				mCurrentState = State::eWalk;
			}
			else if (positiveRange > playerPos.x&& playerPos.x > getPosition().x)
			{
				m_WalkSpeed = m_WalkSpeed;
				mCurrentState = State::eWalk;
			}
		}

		break;
	}

	case State::eWalk:
	{
		m_textureMapName = "alienWalk";

		m_Velocity.x = m_WalkSpeed;

		if (!simulation.isPlayerAlive())
		{
			//Switch back to idle if player dies.
			mCurrentState = State::eStand;

		}

		break;
	}

	}
}

