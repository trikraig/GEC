#pragma once
#include <HAPI_lib.h>
#include "Rectangle.h"
#include <unordered_map>
#include "Vector2.h"

using namespace HAPISPACE;

//Forward declaration , even though not defined yet, only works with pointers and references due to memory size
class Sprite;

class Visualisation
{
private:

	//Singlton implentation
	static Visualisation* _instance;
	Visualisation();
	~Visualisation();

	int m_screenHeight{ 0 }, m_screenWidth{ 0 };
	BYTE* m_screen = nullptr;

	std::unordered_map<std::string, Sprite* > m_spriteMap;
	Rectangle* screenRect{ nullptr };

public:

	static Visualisation& GetInstance();
	
	bool Initialise(const int &screenHeight, const int &screenWidth);

	bool CreateSprite(const std::string& texFilename, const std::string& spriteName, const int& numberOfFrames);

	void ClearToColour(HAPISPACE::HAPI_TColour& col);
	void ClearToGray(BYTE grey = 0);
	void RenderFast(const std::string& filename, const float& posX, const float& posY);
	void Render(const std::string& spriteName,  const Vector2& position, const Vector2& oldPosition, float s);

	int findEntityWidth(const std::string& filename);
	int findEntityHeight(const std::string& filename);



};


//Access system through keyword VIZ
#define VIZ Visualisation::GetInstance()

