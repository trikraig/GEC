#include "Entity.h"
#include <cmath>
#include <iostream>
#include "Rectangle.h"
#include "SoundManager.h"

Entity::Entity(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side) : m_textureMapName(textureMapName), m_health(health), m_damage(damage)
{
	if (side == 0)
	{
		m_side = eSide::eNeutral;
	}
	else if (side == 1)
	{
		m_side = eSide::ePlayer;
	}
	else if (side == 2)
	{
		m_side = eSide::eEnemy;
	}

	m_Position.x = posX;
	m_Position.y = posY;
	m_sounds.damage = "Damage";
	m_maxHealth = health;
}

void Entity::Initialise(const int& width, const int& height)
{
	m_width = width;
	m_height = height;
}


void Entity::Update(Simulation& simulation)
{
	//VIRTUAL
}

//Updates and saves previous position, acceleration and velocity.
void Entity::UpdatePhysics()
{
	m_Position.x += m_Velocity.x;
	m_Position.y += m_Velocity.y;

	m_Velocity.x += m_Acceleration.x;
	m_Velocity.y += m_Acceleration.y;

}

bool Entity::Overlaps(Simulation& simulation, Entity* other)
{

	Rectangle a((float)m_height, (float)m_width, m_Position.x, m_Position.y);
	Rectangle b((float)other->m_height, (float)other->m_width, other->m_Position.x, other->m_Position.y);

	if (a.right <b.left || a.right > b.right || a.bottom < b.top || a.top > b.bottom || other->getIsAlive() == false)
	{
		//No collision
		return false;
	}

	else
	{
		//Both entities take damage, also used to destroy bullets.
		TakeDamage(simulation, other->m_damage);
		other->TakeDamage(simulation, m_damage);
		return true;
	}
}

void Entity::setGrounded()
{
	//Currently only used in EntityPlayer
}

void Entity::TakeDamage(Simulation& simulation, const int& damageAmount)
{
	SMM.PlaySound(m_sounds.damage);
	

	m_health -= damageAmount;


	//Primitive score mechanic.
	if (m_type == eEntityType::eTypeEnemy)
	{
		simulation.increaseScore(damageAmount);
	}

	if (m_health <= 0)
	{
		SMM.PlaySound(m_sounds.death);
		simulation.SpawnExplosion(1000, m_Position.x, m_Position.y);
		isAlive = false;
		if (this->getIsPlayer())
		{
			simulation.GameOver();
		}

	}
	else
	{
		m_Position = m_OldPosition;
		simulation.SpawnExplosion(50, m_Position.x, m_Position.y);
	}
}

void Entity::setPosition(const Vector2& newPos)
{
	m_OldPosition = m_Position;
	m_Position = newPos;
	return;
}

bool Entity::getIsAlive()
{
	return isAlive;
}

eEntityType Entity::getEntityType()
{
	return m_type;
}

Vector2 Entity::getPosition()
{
	return m_Position;
}

Vector2 Entity::getOldPosition()
{
	return m_OldPosition;
}

int Entity::getHealth()
{
	return m_health;
}

eSide Entity::GetSide()
{
	return m_side;
}

std::string Entity::getTexID()
{
	return m_textureMapName;
}

float Entity::getPosX()
{
	return m_Position.x;
}

float Entity::getPosY()
{
	return m_Position.y;
}

float Entity::getIsPlayer()
{
	return isPlayer;
}

Vector2 Entity::getVelocity()
{
	return m_Velocity;
}

