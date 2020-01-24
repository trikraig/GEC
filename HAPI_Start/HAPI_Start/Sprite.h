#pragma once
#include <HAPI_lib.h>
#include "Rectangle.h"

using namespace HAPISPACE;

class Sprite
{
private:

	int m_width{ 0 }, m_height{ 0 };
	std::string texName;
	BYTE* m_data{ nullptr };
	int m_numberOfFrames{ 1 };
	int m_frameNumber{ 1 };
	int m_cleanUpSize{ 2 };
	int m_animationSpeed{ 100 };
	float preTime{ 0 };
	const float kFrameInterval{ 100 };

public:

	Sprite();
	~Sprite();

	bool Load(const std::string& filename, const int& numberOfFrames);
	void Render(const Rectangle& destRect, int x, int y, BYTE* screen, const int& screenHeight, const int& screenWidth, const int& frameNumber);
	void RenderFast(const float& x, const float& y, BYTE* screen, const int& screenHeight, const int& screenWidth);
	int getWidth();
	int getHeight();

};

