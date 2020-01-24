#pragma once
#include <cmath>

class Vector2
{

public:

float x{ 0 }, y{ 0 };

//Constructors
Vector2() = default;
Vector2(const float& newX, const float& newY);

//Operators - rhs right hand side.
Vector2 operator + (const Vector2& rhs) const;
Vector2 operator - (const Vector2& rhs) const;
Vector2 operator * (float rhs) const;

//Helper Functions
Vector2 Normalise();
		

};

