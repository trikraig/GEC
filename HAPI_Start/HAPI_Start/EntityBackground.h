#pragma once
#include "Entity.h"
class EntityBackground : public Entity
{
public:

	EntityBackground(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side);
	void Update(Simulation& simulation) override final;
};

