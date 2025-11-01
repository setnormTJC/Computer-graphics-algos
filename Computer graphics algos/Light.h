#pragma once
#include "Vec4.h"
/*Composed of a position, intensity, and direction, presumably*/
class Light
{
	Vec4 position; 
	float intensity{}; //conventionally set between 0.0f and 1.0f

public: 
	Light() = delete; 
	Light(const Vec4& position, float intensity); 


	const Vec4 getPosition() const; 
	const float getIntensity() const;
};

