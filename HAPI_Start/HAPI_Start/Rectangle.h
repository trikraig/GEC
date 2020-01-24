#pragma once
#include <algorithm>
//Helper class to aid calculation of clipping collisions 
class Rectangle
{

public:

	int left{ 0 }, right{ 0 }, top{ 0 }, bottom{ 0 };

	Rectangle(float height, float width, float newPosX, float newPosY);

	int Width() const;
	int Height() const;

	bool CompletelyOutside(const Rectangle& other);
	bool CompletelyInside(const Rectangle& other);

	void ClipTo(const Rectangle& other);
	
	void Translate(const int &x, const int &y);
	void TranslateBack(const int& x, const int& y);

	
	
};

