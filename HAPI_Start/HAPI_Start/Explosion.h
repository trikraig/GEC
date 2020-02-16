#pragma once
#include "Entity.h"
class Explosion : public Entity
{
	int m_explosionID;
	Vector2 m_origin{ 0,0 };
	float m_explosionLength{ 1000 };
	float m_timeSinceSpawned{ 0 };

public:

	Explosion(const std::string& textureMapName, const float& posX, const float& posY, const int& id);
	void Update(Simulation& simulation) override final;
	void Spawn(const float& length, const Vector2& position);

};

