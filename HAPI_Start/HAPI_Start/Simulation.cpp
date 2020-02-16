#include "Simulation.h"
#include "Visualisation.h"
#include "Entity.h"
#include "EntityBackground.h"
#include "EntityPlayer.h"
#include "EnemyWalker.h"
#include "Bipedal.h"
#include "EnityBullet.h"
#include "Explosion.h"
#include "Vector2.h"
#include <cassert>
#include "SoundManager.h"



Simulation::Simulation(const int& width, const int& height) : m_screenWidth(width), m_screenHeight(height)
{
	leftScreenPos = 0;
	rightScreenPos = 0;

}

Simulation::~Simulation()
{
	for (auto p : allEntities)
	{
		delete p;
	}
	allEntities.clear();
}

bool Simulation::Initialise()
{

	//BEFORE GAME CREATION

	SMM.Initialise();

	VIZ.Initialise(m_screenHeight, m_screenWidth);

	CreateSprites();

	//AFTER GAME START - DELETE ALL EXISTING ENTITIES AND START AGAIN

	NewGame();

	HAPI.SetShowFPS(true);

	return true;
}

//Sprites Creation
void Simulation::CreateSprites()
{

	//VIZ.CreateSprite("Data\\space.png", "background", 1);
	VIZ.CreateSprite("Data\\Environments\\another-world\\PNG\\layered\\composed-bg.png", "background", 1);

	//Player
	VIZ.CreateSprite("Data\\Sprites\\space-marine\\PNG\\space-marine-idle.png", "playerIdle", 4);
	VIZ.CreateSprite("Data\\Sprites\\space-marine\\PNG\\space-marine-run.png", "playerRun", 11);
	VIZ.CreateSprite("Data\\Sprites\\space-marine\\PNG\\space-marine-jump.png", "playerJump", 6);
	VIZ.CreateSprite("Data\\Sprites\\space-marine\\PNG\\space-marine-shoot.png", "playerShoot", 2);
	VIZ.CreateSprite("Data\\Sprites\\space-marine\\PNG\\space-marine-die.png", "playerDie", 2);

	//Enemy
	
	VIZ.CreateSprite("Data\\Sprites\\alien-walking-enemy\\PNG\\alien-enemy-idle.png", "alienIdle", 4);
	VIZ.CreateSprite("Data\\Sprites\\alien-walking-enemy\\PNG\\alien-enemy-walk.png", "alienWalk", 6);

	VIZ.CreateSprite("Data\\Sprites\\bipedal-Unit\\PNG\\Sprites\\bipedal-unit1.png", "bipedalIdle", 1);
	VIZ.CreateSprite("Data\\Sprites\\bipedal-Unit\\PNG\\bipedal-Unit.png", "bipedalWalk", 7);
	//Bullet
	VIZ.CreateSprite("Data\\Sprites\\space-marine\\PNG\\bullet.png", "bullet", 2);
	//Explosions
	VIZ.CreateSprite("Data\\Sprites\\space-marine\\PNG\\wall-impact-blast.png", "bulletImpact", 2);
	VIZ.CreateSprite("Data\\Sprites\\Explosion\\spritesheet\\explosion-animation.png", "explosion", 9);
	
}

//Creates entities and acts as a reset when in game over state.
void Simulation::NewGame()
{
	m_playerScore = 0;

	//Entities Creation
	//Would maybe have a reinitialise function for entities but easier currently to just clear and start again.
	allEntities.clear();

	EntityBackground* background1 = new EntityBackground("background", 0, 0, 0, 0, 0);
	allEntities.push_back(background1);

	EntityBackground* background2 = new EntityBackground("background", 0, 0, (float)-(VIZ.findEntityWidth("background")), 0, 0);
	allEntities.push_back(background2);

	EntityBackground* background3 = new EntityBackground("background", 0, 0, (float)(VIZ.findEntityWidth("background")), 0, 0);
	allEntities.push_back(background3);

	EntityBackground* background4 = new EntityBackground("background", 0, 0, 2 * ((float)VIZ.findEntityWidth("background")), 0, 0);
	allEntities.push_back(background4);

	//Record Player Position in allEntities
	m_player = allEntities.size();
	EntityPlayer* player = new  EntityPlayer("playerIdle", 5, 1, 100, 100, 1);
	allEntities.push_back(player);

	//Generate multiple enemies. Could space manually throughout level but just creating a simple gauntlet for the demo.
	for (float i = 0; i < 10; i++)
	{
		EnemyWalker* walker = new EnemyWalker("alienIdle", 1, 1, 500 + (i * 100), 600, 2);
		allEntities.push_back(walker);
	}

	//Acts as a barrier to guide player towards the end of the gauntlet in this demo.
	Bipedal* bipedal = new Bipedal("bipedalIdle", 10, 5, -400, 600, 2);
	allEntities.push_back(bipedal);

	//Bullets Creation

	m_bulletsStart = allEntities.size();

	for (int i = 1; i < m_totalBullets + 1; i++)
	{

		EnityBullet* bullet = new EnityBullet("bullet", 1, 1, 0, 0, 0, i);

		allEntities.push_back(bullet);
	}

	//Explosions Creation

	m_explosionsStart = allEntities.size();

	for (int i = 1; i < m_totalExplosions + 1; i++)
	{
		Explosion* explosion = new Explosion("explosion", 0, 0, i);
		allEntities.push_back(explosion);
	}

	//Getting size of entity via texture look up in viz
	for (auto& p : allEntities)
	{
		p->Initialise(VIZ.findEntityWidth(p->getTexID()), VIZ.findEntityHeight(p->getTexID()));
	}

	SMM.PlaySoundLoop("level00");
}

//Main gameplay loop
void Simulation::Run()
{
	//Interpolation integration - Not Complete due to issues with the render function
	float lastTimeTicked{ 0 };

	while (HAPI.Update())
	{
		float timeSinceLastTick{ static_cast <float> (HAPI.GetTime()) - lastTimeTicked };

		const HAPISPACE::HAPI_TControllerData& gamepadData = HAPI.GetControllerData(0);

		const HAPISPACE::HAPI_TKeyboardData& keyData = HAPI.GetKeyboardData();

		switch (state)
		{
			//MAIN MENU
		case eMain:

			VIZ.ClearToGray();
			RenderMainMenu();

			if (keyData.scanCode[HK_RETURN] || gamepadData.isAttached && gamepadData.digitalButtons[HK_DIGITAL_START])
			{

				state = GameState::ePlay;

			}

			break;

		case ePlay:

			//Simulation-------------------------------------------------------------------------------------------------------------------------------------------

			if (timeSinceLastTick >= k_tickTime)
			{
				for (auto& p : allEntities)
				{

					if (p->getIsAlive())
					{
						//Updates all entities
						p->Update(*this);
						//Aligning window to player position
						if (p->getIsPlayer())
						{
							m_worldX = -p->getPosX() + m_screenWidth / 2;
						}
					}

					lastTimeTicked = static_cast <float> (HAPI.GetTime());
					//Checking collisions
					//Checking all other entities if is enemy
					if (p->GetSide() != eSide::eNeutral)
					{
						for (auto& o : allEntities)
						{
							if (p->GetSide() == eSide::ePlayer && o->GetSide() == eSide::eEnemy || p->GetSide() == eSide::eEnemy && o->GetSide() == eSide::ePlayer && p->getIsAlive() && o->getIsAlive())
							{
								p->Overlaps(*this, o);
							}
						}
					}

					timeSinceLastTick = 0;

				}


			}

			s = timeSinceLastTick / k_tickTime;

			//Check between 0 and 1.
			assert(s >= 0 && s <= 1.0f);


			VIZ.ClearToGray();

			//Rendering------------------------------------------------------------------------------------------------------------------------------------


			//Render entities
			for (int i = 0; i < allEntities.size(); i++)
			{
				if (allEntities[i]->getIsAlive())
				{
					Vector2 worldOldPos{ Vector2(m_worldX + allEntities[i]->getOldPosition().x, (m_worldY + allEntities[i]->getOldPosition().y)) };
					Vector2 worldNewPos{ Vector2(m_worldX + allEntities[i]->getPosX(),(m_worldY + allEntities[i]->getPosY())) };
					VIZ.Render(allEntities[i]->getTexID(), worldNewPos, worldOldPos, s);
				}

			}
			RenderGameUI();
			break;

			//GAME OVER screen
		case eGameOver:

			SMM.StopAllLoopedSound();
			VIZ.ClearToGray();
			RenderMenuGameOver();
			if (keyData.scanCode[HK_RETURN] || gamepadData.isAttached && gamepadData.digitalButtons[HK_DIGITAL_START])
			{
				//Resets the game
				NewGame();
				state = GameState::eMain;

			}
			break;
		default:
			break;
		}

	}
}

bool Simulation::SpawnBullet(const float& directionX, const float& directionY, const int& side, const float& positionX, const float& positionY)
{
	//Finds a bullet that is not being used.
	for (size_t i = m_bulletsStart; i < allEntities.size(); i++)
	{

		if (!allEntities[i]->getIsAlive())
		{
			SMM.PlaySound("Shoot");

			//Must check is of type Entity bullet before attempting to cast down.
			assert(allEntities[i]->getEntityType() == eEntityType::eTypeBullet);
			(static_cast <EnityBullet*> (allEntities[i])->Shoot(side, Vector2(directionX, directionY), Vector2(positionX, positionY)));
			return true;
		}

	}
	return false;

}

bool Simulation::SpawnExplosion(const float& length, const float& positionX, const float& positionY)
{
	//Finds an explosion that is not being used.
	for (size_t i = m_explosionsStart; i < allEntities.size(); i++)
	{
		if (!allEntities[i]->getIsAlive())
		{
			SMM.PlaySound("Explosion"); // Create Sound.

			//Same with bullet, must check of type entity explosion before attempting to cast down.
			assert(allEntities[i]->getEntityType() == eEntityType::eTypeExplosion);
			(static_cast <Explosion*> (allEntities[i])->Spawn(length, Vector2(positionX, positionY)));
			return true;
		}
	}
	return false;
}

int Simulation::getScreenWidth()
{
	return m_screenWidth;
}

Vector2 Simulation::getPlayerPosition()
{
	return allEntities[m_player]->getPosition();
}

bool Simulation::isPlayerAlive()
{
	return allEntities[m_player]->getIsAlive();
}

void Simulation::RenderMenuGameOver()
{

	if (playerHasWon)
	{
		SMM.PlaySound("Win");
		HAPI.RenderText((m_screenWidth / 4), m_screenHeight / 2, HAPI_TColour(255, 0, 0), "YOU WIN, SCORE: " + std::to_string(m_playerScore), fontSize);
	}
	else
	{
		
		HAPI.RenderText((m_screenWidth / 4), m_screenHeight / 2, HAPI_TColour(255, 0, 0), "GAME OVER, SCORE: " + std::to_string(m_playerScore), fontSize);


	}

	HAPI.RenderText((m_screenWidth / 4), (m_screenHeight / 2) + 2 * fontSize, HAPI_TColour(255, 0, 0), "Press Enter Key or Start to Continue", fontSize);


}

void Simulation::RenderMainMenu()
{
	HAPI.RenderText((m_screenWidth / 4), m_screenHeight / 2, HAPI_TColour(255, 0, 0), "ALIEN BLARG - GEC PROJECT", 2 * fontSize);
	HAPI.RenderText((m_screenWidth / 4), m_screenHeight / 2 + 2 * fontSize, HAPI_TColour(255, 0, 0), "Press Enter Key or Start to Continue", fontSize);
	HAPI.RenderText((m_screenWidth / 4), m_screenHeight / 2 + 4 * fontSize, HAPI_TColour(255, 0, 0), "By Craig Palmer", fontSize);
}

void Simulation::GameOver()
{
	playerHasWon = false;
	state = GameState::eGameOver;
}

void Simulation::setWin()
{
	playerHasWon = true;
	state = GameState::eGameOver;
}

float Simulation::getWorldEndZone()
{
	return m_worldEndZone;
}

//UI during game play, to be future expanded upon
void Simulation::RenderGameUI()
{

	HAPI.RenderText(100, 100, HAPI_TColour(255, 0, 0), "Health: " + std::to_string(allEntities[m_player]->getHealth()), fontSize);
	HAPI.RenderText(m_screenWidth - 100, 100, HAPI_TColour(255, 0, 0), "Score: " + std::to_string(m_playerScore), fontSize);
}

void Simulation::increaseScore(const int& amount)
{
	m_playerScore += amount;
	return;
}





