#pragma once
#include <string>
#include "Vector2.h"
#include "myEnum.h"
#include "Simulation.h"

//Store sound map names for each entity. 
struct Sounds
{
	std::string death;
	std::string jump;
	std::string damage;
	std::string enemyDiscovered;

};

//Which side an entity is on.
enum  eSide
{
	eNeutral, ePlayer, eEnemy,
};

//Not all are used but ability is there.
enum State
{
	eStand, eWalk, eJumping, eDie, eShooting
};

class Entity
{
private:

	Vector2 m_Position{ 0,0 };
	Vector2 m_OldPosition{ 0,0 };

protected:

	eSide m_side{ eSide::eNeutral };

	State mCurrentState{ State::eStand };

	eEntityType m_type{};

	int m_health{ 0 };
	int m_maxHealth{ 0 };
	int m_damage{ 0 };

	int m_width{ 0 };
	int m_height{ 0 };


	Vector2 m_Acceleration{ 0,0 };
	Vector2 m_Velocity{ 0,0 };

	std::string m_textureMapName;

	bool isAlive{ true };
	bool isPlayer{ false };

	Sounds m_sounds;


public:

	Entity(const std::string& textureMapName, const int& health, const int& damage, const float& posX, const float& posY, const int& side);

	virtual void Initialise(const int& width, const int& height);
	virtual void Update(Simulation& simulation) = 0;
	virtual bool Overlaps(Simulation& simulation, Entity* other);
	virtual void setGrounded();

	void UpdatePhysics();

	void TakeDamage(Simulation& simulation, const int& damageAmount);

	//POSITIONS
	Vector2 getPosition();
	Vector2 getOldPosition();
	void setPosition(const Vector2& newPos);
	//GETTERS
	int getHealth();
	virtual eSide GetSide();
	std::string getTexID();
	float getPosX();
	float getPosY();
	float getIsPlayer();
	Vector2 getVelocity();
	bool getIsAlive();
	eEntityType getEntityType();


};