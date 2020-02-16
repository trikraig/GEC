#include "Explosion.h"
#include <HAPI_lib.h>

Explosion::Explosion(const std::string& textureMapName, const float& posX, const float& posY, const int& id) : Entity(textureMapName, 1, 0, posX, posY, 0)
{
	m_explosionID = id;
	//Goes straight into pool
	isAlive = false;
	m_type = eEntityType::eTypeExplosion;

}

void Explosion::Update(Simulation& simulation)
{
	float currentTime = static_cast <float> (HAPI.GetTime());

	if (currentTime > m_explosionLength + m_timeSinceSpawned)
	{
		isAlive = false;
		//Could use as a trigger for a multiple step explosion.
	}

}

//Retrieve explosion from pool, assigns length and position.
void Explosion::Spawn(const float& length, const Vector2& position)
{
	isAlive = true;
	m_explosionLength = length;
	setPosition(Vector2(position.x - m_width / 2, position.y - 2 * m_height));
	m_timeSinceSpawned = static_cast <float>  (HAPI.GetTime());
}
