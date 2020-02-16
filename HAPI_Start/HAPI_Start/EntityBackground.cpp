#include "EntityBackground.h"

EntityBackground::EntityBackground(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side) : Entity(textureMapName, health, damage, posX, posY, side)
{
	m_type = eEntityType::eTypeBackground;
}

void EntityBackground::Update(Simulation& simulation)
{
	//Could include ability to scroll, parallax, animate etc...
}

