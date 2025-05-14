#include "TriangleShape.h"

TriangleShape::TriangleShape() : baseLength(0), height(0), quickness(500) {
    vertexA = glm::vec3(0, 0, 0);
    vertexB = glm::vec3(0, 0, 0);
    vertexC = glm::vec3(0, 0, 0);
    basePosition = glm::vec3(0, 0, 0);
}

void TriangleShape::draw() {
    ofPushMatrix();
    ofMultMatrix(getTransform());

    ofBeginShape();
    ofVertex(vertexA);  // Bottom left
    ofVertex(vertexD);  // Bottom middle dent
    ofVertex(vertexB);  // Bottom right
    ofVertex(vertexC);  // Tip of the spaceship
    ofEndShape(true);

    //Player outline
    ofDrawLine(vertexC, vertexA);
    ofDrawLine(vertexC, vertexB);
    ofDrawLine(vertexA, vertexD); // Bottom left to dent
    ofDrawLine(vertexD, vertexB); // Dent to bottom right

    ofPopMatrix();
}

void TriangleShape::initializeTriangle(float height, float base) {

    height = height;

    basePosition.x = position.x;
    basePosition.y = position.y + (base / 2);

    vertexA = glm::vec3(-base / 2, base / 2, 0);
    vertexB = glm::vec3(base / 2, base / 2, 0);
    vertexC = glm::vec3(0, -height, 0);
    vertexC = glm::vec3(0, -height * 0.8, 0);
}

bool TriangleShape::inside(glm::vec3 p0) {
    glm::vec3 p = glm::inverse(getTransform()) * glm::vec4(p0, 1);
    glm::vec3 v1 = glm::normalize(vertexA - p);
    glm::vec3 v2 = glm::normalize(vertexB - p);
    glm::vec3 v3 = glm::normalize(vertexC - p);

    float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
    float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
    float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));

    return (a1 < 0 && a2 < 0 && a3 < 0);
}

void TriangleShape::moveForward() {
    //position = position + heading() * quickness;
    applyForce(heading() * 1.5 * quickness);
}

void TriangleShape::moveBackwards() {
    //position = position - heading() * quickness;
    applyForce(-heading() * 1.5 * quickness);
}

void TriangleShape::turnRight() {
    //angle += 5;
    applyTorque(1.7f);
}

void TriangleShape::turnLeft() {
    //angle -= 5;
    applyTorque(-1.7f);
}
