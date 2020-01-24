#include "Vector2.h"

Vector2::Vector2(const float& newX, const float& newY) : x(newX), y(newY)
{
}

//Addition
Vector2 Vector2::operator+(const Vector2& rhs) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}

//Subtraction
Vector2 Vector2::operator-(const Vector2& rhs) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}
//Multipication
Vector2 Vector2::operator*(float rhs) const
{
	return Vector2(x * rhs, y * rhs);
}

//Normalisation - Efficiency could be improved
Vector2 Vector2::Normalise()
{

	float mag = sqrt(x * x + y * y);
	Vector2 result(x / mag, y / mag);

	return result;
}
