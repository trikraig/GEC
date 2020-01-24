#pragma once
#include <string>
#include <vector>
#include <memory>
#include "myEnum.h"
#include "Vector2.h"


//class Visualisation;
class Entity;

enum GameState
{
	eMain, ePlay, eGameOver
};


class Simulation
{
private:

	int m_screenWidth{ 0 };
	int m_screenHeight{ 0 };

	float m_worldX{ 0 };
	float m_worldY{ 0 };

	//Basic objective in world space to reach.
	float m_worldEndZone{ 2000 };
	bool playerHasWon{ false };
	int m_playerScore{ 0 };
	//Bullet pool management
	int m_totalBullets{ 20 };
	size_t m_bulletsStart{ 0 };
	//Explosion pool management
	int m_totalExplosions{ 10 };
	size_t m_explosionsStart{ 0 };
	//Stores location of player in entities container.
	size_t m_player{ 0 };

	std::vector <Entity*> allEntities;
	int fontSize{ 20 };

	float leftScreenPos{ NULL };
	float rightScreenPos{ NULL };

	//Interpolation 
	const float k_tickTime = 15;
	const float k_spriteInterval = 10;
	float s{ 0 };

	//Starting game state.
	GameState state{ GameState::eMain };


public:

	Simulation(const int& width, const int& height);
	~Simulation();

	bool Initialise();
	void CreateSprites();
	void NewGame();
	void Run();

	bool SpawnBullet(const float& directionX, const float& directionY, const int& side, const float& positionX, const float& positionY);
	bool SpawnExplosion(const float& length, const float& positionX, const float& positionY);

	bool isPlayerAlive();
	Vector2 getPlayerPosition();
	void increaseScore(const int& amount);

	void setWin();
	void GameOver();

	int getScreenWidth();
	float getWorldEndZone();

	void RenderMenuGameOver();
	void RenderMainMenu();
	void RenderGameUI();




};

