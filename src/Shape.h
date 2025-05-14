#pragma once

#include "ofMain.h"

class Shape {
public:
    // methods
    Shape() {}
    virtual void draw() {}
    virtual bool intersect(glm::vec3 p) { return false; }

    // data
    glm::mat4 Shape::getTransform() {
        glm::mat4 T = glm::translate(glm::mat4(1.0), position);
        glm::mat4 R = glm::rotate(glm::mat4(1.0), glm::radians(angle), glm::vec3(0, 0, 1));
        glm::mat4 S = glm::scale(glm::mat4(1.0), scale);
        return (T * R * S);

    }

    // Heading Vector
    glm::vec3 heading() {
        glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(angle), glm::vec3(0, 0, 1));
        return glm::normalize(rot * glm::vec4(glm::vec3(0, -1, 0), 1));
    }

    // Draws the heading vector
    void drawLine() {
        ofDrawLine(position, position + heading() * (250 * scale));
    }

    bool inside() {

    }

    // Data
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
    float angle;
};