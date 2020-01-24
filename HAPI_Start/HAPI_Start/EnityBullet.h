#pragma once
#include "Entity.h"

class EnityBullet : public Entity
{
private: 
	
	int m_bulletID{ 0 };
	float m_maxVelocity{ 5 };
	Vector2 m_origin{ 0,0 };
	

public:

	EnityBullet(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side, const int& id);
	void Update(Simulation& simulation) override final;
	void Shoot(const int& side, const Vector2& direction, const Vector2& position);
};

