#include "Sprite.h"


Sprite::Sprite()
{

	preTime = static_cast <float> (HAPI.GetTime());
}

Sprite::~Sprite()
{
	delete[] m_data;
}

bool Sprite::Load(const std::string& filename, const int& numberOfFrames) //Pass in number of frames
{
	//frame size is going to be m_width / number of frames. 
	if (!HAPI.LoadTexture(filename, &m_data, m_width, m_height))
	{
		//ERROR - TEXTURE NOT FOUND
		HAPI.UserMessage("TEXTURE NOT FOUND IN DIRECTORY", "Sprite Load Error");
		return false;
	}

	m_numberOfFrames = numberOfFrames;
	return true;
}

void Sprite::Render(const Rectangle& destRect, int x, int y, BYTE* screen, const int& screenHeight, const int& screenWidth, const int& frameNumber) //pass in frame number to render
{

	float currentTime = (float)HAPI.GetTime();
	//Animation frame calculation
	float frameWidth = static_cast <float> ((m_width / m_numberOfFrames));
	float frameOffset = m_frameNumber * frameWidth;

	Rectangle frameRect(static_cast <float> ((m_height - 5)), frameWidth, m_width - frameOffset, 0);

	frameRect.Translate(x, y);

	if (!frameRect.CompletelyOutside(destRect))
	{
		frameRect.ClipTo(destRect);
		frameRect.TranslateBack(x, y);

		if (x < 0) x = 0;
		if (y < 0) y = 0;

#
		size_t texOffset = ((size_t)frameRect.left + (size_t)frameRect.top * (size_t)m_width) * 4;
		BYTE* texturePntr = m_data + texOffset;
		unsigned int offset = (x + y * screenWidth) * 4;
		BYTE* screenPntr = screen + offset;


		for (int j = 0; j < frameRect.Height(); j++)
		{
			for (int k = 0; k < frameRect.Width(); k++)
			{
				HAPI_TColour* sourceCol = (HAPI_TColour*)texturePntr;
				HAPI_TColour* destCol = (HAPI_TColour*)screenPntr;

				if (sourceCol->alpha == 255)
				{
					*destCol = *sourceCol;
				}

				else if (sourceCol->alpha > 0)
				{
					float alpha = sourceCol->alpha / 255.0f;

					destCol->red = static_cast <HAPISPACE::BYTE> ((alpha * sourceCol->red + (1.0f - alpha) * destCol->red));
					destCol->green = static_cast <HAPISPACE::BYTE> ((alpha * sourceCol->green + (1.0f - alpha) * destCol->green));
					destCol->blue = static_cast <HAPISPACE::BYTE> ((alpha * sourceCol->blue + (1.0f - alpha) * destCol->blue));

				}

				screenPntr += 4;
				texturePntr += 4;

			}

			screenPntr += ((size_t)screenWidth - (size_t)frameRect.Width()) * 4;
			texturePntr += ((size_t)m_width - (size_t)frameRect.Width()) * 4;
		}

	}

	if (currentTime > preTime + m_animationSpeed)
	{
		//Steps through animation 
		m_frameNumber++;

		preTime = static_cast <float> (HAPI.GetTime());
	}


	if (m_frameNumber > m_numberOfFrames)
	{
		//Reset animation back to first frame.
		m_frameNumber = 1;
	}

}

void Sprite::RenderFast(const float& x, const float& y, BYTE* screen, const int& screenHeight, const int& screenWidth)
{

	BYTE* screenPntr = screen;
	BYTE* texturePntr = m_data;

	for (int y = 0; y < m_height; y++)
	{
		memcpy(screenPntr, texturePntr, (size_t)m_width * 4);

		// Move texture pointer to next line
		texturePntr += (size_t)m_width * 4;

		// Move screen pointer to next line
		screenPntr += (size_t)screenWidth * 4;
	}
}

int Sprite::getWidth()
{
	return m_width / m_numberOfFrames;
}

int Sprite::getHeight()
{
	return m_height / m_numberOfFrames;
}

