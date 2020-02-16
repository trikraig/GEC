#pragma once
#include "Entity.h"
class Bipedal : public Entity
{
private:

	float m_WalkSpeed{ 2 };
	float m_ShootingRange{ 250 };
	float m_ShootingInterval{ 1000 };
	float m_timeLastFired{ 0 };

public:

	Bipedal(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side);
	void Update(Simulation& simulation) override final;

};

