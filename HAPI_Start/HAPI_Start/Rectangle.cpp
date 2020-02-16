#include "Rectangle.h"

Rectangle::Rectangle(float height, float width, float newPosX, float newPosY)
{
	right = static_cast <int> ((width + newPosX));
	bottom = static_cast <int> ((height + newPosY));
	left = static_cast <int> ((newPosX));
	top = static_cast <int> ((newPosY));

}

int Rectangle::Width() const
{
	int temp{ right - left };

	return temp;
}

int Rectangle::Height() const
{
	int temp{ bottom - top };

	return temp;
}

bool Rectangle::CompletelyOutside(const Rectangle& other)
{
	if (left > other.right || top > other.bottom || right < other.left || bottom < other.top)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Rectangle::CompletelyInside(const Rectangle& other)
{
	if (left < other.right || top < other.bottom || right > other.left || bottom > other.top)
	{
		return true;
	}

	else
	{
		return false;
	}
}

void Rectangle::ClipTo(const Rectangle& other)
{


	left = std::max(left, other.left);
	right = std::min(right, other.right);
	top = std::max(top, other.top);
	bottom = std::min(bottom, other.bottom);


}

void Rectangle::Translate(const int& x, const int& y)
{
	top += y;
	left += x;
	right += x;
	bottom += y;
}

void Rectangle::TranslateBack(const int& x, const int& y)
{
	top -= y;
	left -= x;
	right -= x;
	bottom -= y;

}


