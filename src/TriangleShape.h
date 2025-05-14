#pragma once

#include "ofMain.h"
#include "DynamicShape.h"

class TriangleShape : public DynamicShape {
public:
    TriangleShape();

    void draw();
    void initializeTriangle(float height, float base);
    bool inside(glm::vec3 p0);
    void moveForward();
    void moveBackwards();
    void turnRight();
    void turnLeft();

    float baseLength;
    float height;
    float quickness;
    glm::vec3 vertexA;
    glm::vec3 vertexB;
    glm::vec3 vertexC;
    glm::vec3 vertexD;
    glm::vec3 basePosition;
};
