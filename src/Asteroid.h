#pragma once

#include "ofMain.h"
#include "DynamicShape.h"

class Asteroid : public DynamicShape {
public:
	Asteroid();

	void draw();
	void integrate();
	vector<Asteroid> split();

	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec3 point3;
	glm::vec3 point4;
	glm::vec3 point5;
	glm::vec3 point6;
	glm::vec3 point7;
	glm::vec3 point8;
	glm::vec3 point9;
	glm::vec3 point10;
	float size;
	float side;
	float maxSpeed;
	float radius;

	int frameCounter;
	int frameSkip;

	float minRadius1;
	float minRadius2;
	float maxRadius1;
	float maxRadius2;
};

class MiniAsteroid : public Asteroid {
public:
	MiniAsteroid();
};