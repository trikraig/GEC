#include "Visualisation.h"
#include "Sprite.h"


Visualisation* Visualisation::_instance{ nullptr };
//Ensures only one instance
Visualisation& Visualisation::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Visualisation;

	}

	return *_instance;
}

Visualisation::Visualisation()
{
}

Visualisation::~Visualisation()
{
	delete screenRect;
	delete m_screen;
	for (auto p : m_spriteMap)
	{
		delete p.second;
	}
	m_spriteMap.clear();
	delete this;
}

bool Visualisation::Initialise(const int& screenHeight, const int& screenWidth)
{
	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;

	if (!HAPI.Initialise(m_screenWidth, m_screenHeight, "ALIEN BLARG"))
	{
		return false;
	}

	m_screen = HAPI.GetScreenPointer();

	screenRect = new Rectangle(static_cast <float> (m_screenHeight), static_cast <float> (m_screenWidth), 0, 0);

	return true;
}

bool Visualisation::CreateSprite(const std::string& filename, const std::string& name, const int& numberOfFrames)
{
	Sprite* newSprite = new Sprite;

	if (!newSprite->Load(filename, numberOfFrames))
	{
		delete newSprite;
		return false;
	}

	m_spriteMap[name] = newSprite;

	return true;
}


void Visualisation::RenderFast(const std::string& name, const float& posX, const float& posY)
{
	//Check if map contains the requested sprite.
	if (m_spriteMap.find(name) == m_spriteMap.end())
	{
		std::cerr << "Sprite does not exist: " << name << std::endl;
		return;
	}

	m_spriteMap.at(name)->RenderFast(posX, posY, m_screen, m_screenHeight, m_screenWidth);

}

void Visualisation::Render(const std::string& name, const Vector2& position, const Vector2& oldPosition, float s)
{
	//Check if map contains the requested sprite.
	if (m_spriteMap.find(name) == m_spriteMap.end())
	{
		//Error Message
		std::cerr << "Cannot load sprite: " << name << std::endl;
		return;
	}

	//Calculate Interpolation

	Vector2 interPos{ oldPosition + (position - oldPosition) * s };

	m_spriteMap.at(name)->Render(*screenRect, static_cast <int> (position.x), static_cast <int> (position.y), m_screen, m_screenHeight, m_screenWidth, 1);
}

int Visualisation::findEntityWidth(const std::string& filename)
{
	if (m_spriteMap.find(filename) == m_spriteMap.end())
	{
		//Error Message
		std::cerr << "Sprite does not exist: " << filename << std::endl;
		return 0;
	}

	return m_spriteMap.at(filename)->getWidth();


}

int Visualisation::findEntityHeight(const std::string& filename)
{
	if (m_spriteMap.find(filename) == m_spriteMap.end())
	{
		//Error Message
		std::cerr << "Sprite does not exist: " << filename << std::endl;
		return 0;
	}

	return m_spriteMap.at(filename)->getHeight();
}



void Visualisation::ClearToColour(HAPISPACE::HAPI_TColour& col)
{
	BYTE* temp = m_screen;

	for (int i = 0; i < (m_screenHeight * m_screenWidth); i++)
	{
		memcpy(temp, &col, 4);
		temp += 4;
	}

}

void Visualisation::ClearToGray(BYTE grey)
{
	memset(m_screen, grey, (size_t)m_screenWidth * m_screenHeight * 4);
}


