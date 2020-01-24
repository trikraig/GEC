#pragma once
#include "Entity.h"

//Crawer / Zombie enemy entity type.
class EnemyWalker : public Entity
{
private:

	float m_WalkSpeed{ 1 };
	float m_attackRange{ 250 };

public:

	EnemyWalker(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side);
	void Update(Simulation& simulation) override final;


};

