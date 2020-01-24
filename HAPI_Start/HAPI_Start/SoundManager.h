#pragma once
#include <unordered_map>
#include <HAPI_lib.h>

struct Sound
{
	std::string filename;
};

class SoundManager
{
private:

	//Singleton implentation 
	static SoundManager* _instance;
	SoundManager();
	~SoundManager();

	//Used to store looping sounds such as ambience, music etc
	std::vector <int> allActiveSounds;

	std::unordered_map <std::string, Sound> m_soundMap;

public:

	static SoundManager& GetInstance();

	bool Initialise();

	bool CreateSound(std::string soundName, std::string soundFileName);
	bool PlaySound(std::string soundName);
	bool PlaySoundLoop(std::string soundName);
	bool StopAllLoopedSound();


};

//Keyword to access
#define SMM SoundManager::GetInstance()