#pragma once

#include "ofMain.h"
#include "DynamicShape.h"

class Projectile : public DynamicShape{
public:
	Projectile();

	void draw();
	void integrate();

	float projSpeed;
};