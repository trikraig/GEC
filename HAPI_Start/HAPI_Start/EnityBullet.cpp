#include "EnityBullet.h"

EnityBullet::EnityBullet(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side, const int& id) : Entity(textureMapName, health, damage, posX, posY, side)
{
	m_bulletID = id;
	isAlive = false;
	m_type = eEntityType::eTypeBullet;

}

void EnityBullet::Update(Simulation& simulation)
{
	//Ensures continuous movement
	m_Velocity.x = m_maxVelocity;

	UpdatePhysics();

	//Check if gone off screen. Destroy 
	if (getPosition().x > m_origin.x + simulation.getScreenWidth() || getPosition().x < m_origin.x - simulation.getScreenWidth())
	{
		isAlive = false;
	}

}

//Retrieve bullet from pool, assigns side, position and direction of travel.
void EnityBullet::Shoot(const int& side, const Vector2& direction, const Vector2& position)
{
	isAlive = true;
	m_side = static_cast <eSide> (side);
	m_maxVelocity = m_maxVelocity *  direction.x;
	//m_origin can be used to line up sprite with barrel of weapon etc, not applied in this case but functionality is there if position passed in.
	setPosition(position);
	m_origin = position;

}


