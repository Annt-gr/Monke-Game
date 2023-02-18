#pragma once
#include <iostream>
#include <math.h>

//class used for movement
class Vector2D {
public:
	float x;
	float y;

	//riza 2 deytera
	Vector2D() {
		x = sqrt(2.0f) * 0.5f;
		y = sqrt(2.0f) * 0.5f;
	}

	Vector2D(float xx, float yy) {
		x = xx; y = yy;
	}
	//kanonikopoihsh
	void normalize() {
		float length = sqrt(x * x + y * y);
		if (length == 0)
			std::cout << "SOMETHING WENT WRONG " << std::endl;
		x /= length;
		y /= length;
	}
	//chance x,y depending on rotetion
	void fromAngle(float a) {
		//Convert angles to rad
		float arad = (3.141592653f * (90.0f + a) / 180.0f);
		x = cosf(arad);
		y = -sinf(arad); 
		normalize();
		//std::cout << "Updated Vector " << x << " " << y << std::endl;
	}
};