#include "SoundManager.h"



SoundManager* SoundManager::_instance{ nullptr };

SoundManager& SoundManager::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new SoundManager;
	}

	return *_instance;
}

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
	m_soundMap.clear();
	delete this;
}

bool SoundManager::Initialise()
{

	//Music

	CreateSound("level00", "Data\\Sounds\\Ambience_AlienPlanet_00.wav");

	//Player

	CreateSound("Shoot", "Data\\Sounds\\Laser\\Laser_01.wav");
	CreateSound("Jump", "Data\\Sounds\\Alien\\jump.wav");

	//Enemies
	CreateSound("AlienDeath", "Data\\Sounds\\Alien\\death1.wav");

	//Other Effects
	CreateSound("Damage", "Data\\Sounds\\Alien\\pain100_1.wav");

	CreateSound("RobotDeath", "Data\\Sounds\\Robot_Talk_02.wav");

	CreateSound("RobotActivate", "Data\\Sounds\\Robot_Talk_00.wav");

	CreateSound("Explosion", "Data\\Sounds\\Laser\\Laser_05.wav");

	CreateSound("Win", "Data\\Sounds\\Jingle_Win_00.wav");


	return true;
}

//Adds a sound to the library / sound map
bool SoundManager::CreateSound(std::string soundName, std::string soundFileName)
{


	if (!HAPI.LoadSound(soundFileName))
	{
		std::cerr << "Cannot load sound: " << soundFileName << std::endl;
		return false;
	}

	Sound newSound;

	newSound.filename = soundFileName;

	m_soundMap[soundName] = newSound;

	return true;

}

bool SoundManager::PlaySound(std::string soundName)
{
	if (soundName == "")
	{
		return true;
	}
	else if (m_soundMap.find(soundName) == m_soundMap.end())
	{
		//Error Message
		std::cerr << "Cannot load sound: " << soundName << std::endl;
		return false;
	}

	HAPI.PlaySound(m_soundMap.at(soundName).filename);


	return true;
}

bool SoundManager::PlaySoundLoop(std::string soundName)
{
	if (soundName == "")
	{
		return true;
	}
	else if (m_soundMap.find(soundName) == m_soundMap.end())
	{
		//Error Message
		std::cerr << "Cannot load sound: " << soundName << std::endl;
		return false;
	}

	int loopedMusicID;

	HAPISPACE::HAPI_TSoundOptions options(0.5f, true, 0);

	HAPI.PlaySound(m_soundMap.at(soundName).filename, options, loopedMusicID);

	allActiveSounds.push_back(loopedMusicID);


	return true;
}

//On new game will reset all active sounds.
bool SoundManager::StopAllLoopedSound()
{
	for (auto& i : allActiveSounds)
	{
		HAPI.StopSound(i, false);
	}

	allActiveSounds.clear();

	return true;

}




