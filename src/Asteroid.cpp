#include "Asteroid.h"

Asteroid::Asteroid() {

    birthtime = ofGetElapsedTimef();
    maxSpeed = 200;
    torque = ofRandom(4.0000005, 4.000001);
    float maxRadius = 0;

    frameCounter = 0;
    frameSkip = 1;  // Rotate every 2 frames (adjust to control speed)

    minRadius1 = 20;
    minRadius2 = 50;
    maxRadius1 = 100;
    maxRadius2 = 110;

    float min = ofRandom(minRadius1, minRadius2);
    float max = ofRandom(maxRadius1, maxRadius2);

    // Center pos
    float cx = position.x;
    float cy = position.y;

    // Angles for the 10 vertices of asteroid
    float angles[10] = { 0, 36, 72, 108, 144, 180, 216, 252, 288, 324 };

    size = 10;

    // Convert degrees to radians
    for (int i = 0; i < 10; i++) {
        float angleRad = glm::radians(angles[i]);

        // Get random radius within min/max range
        float radius = ofRandom(min, max);

        if (radius > maxRadius) {
            maxRadius = radius;
        }
        
        // Compute ordered points
        float x = cx + cos(angleRad) * radius;  // x value of vertex 
        float y = cy + sin(angleRad) * radius;  // y value of vertex

        // For each vertex
        switch (i) {
        case 0:  point1 = glm::vec3(x, y, 0); break;
        case 1:  point2 = glm::vec3(x, y, 0); break;
        case 2:  point3 = glm::vec3(x, y, 0); break;
        case 3:  point4 = glm::vec3(x, y, 0); break;
        case 4:  point5 = glm::vec3(x, y, 0); break;
        case 5:  point6 = glm::vec3(x, y, 0); break;
        case 6:  point7 = glm::vec3(x, y, 0); break;
        case 7:  point8 = glm::vec3(x, y, 0); break;
        case 8:  point9 = glm::vec3(x, y, 0); break;
        case 9:  point10 = glm::vec3(x, y, 0); break;
        }
    }

    // Set radius to max radius (used later for collision detection)
    radius = maxRadius;
}

MiniAsteroid::MiniAsteroid() {
    birthtime = ofGetElapsedTimef();
    maxSpeed = 200;
    torque = ofRandom(4.0000005, 4.000001);
    float maxRadius = 0;

    frameCounter = 0;
    frameSkip = 1;  // Rotate every 2 frames (adjust this value to control speed)

    minRadius1 = 20 / 3;
    minRadius2 = 50 / 3;
    maxRadius1 = 100 / 3;
    maxRadius2 = 110 / 3;

    float min = ofRandom(minRadius1, minRadius2);
    float max = ofRandom(maxRadius1, maxRadius2);

    // Center pos
    float cx = position.x;
    float cy = position.y;

    // Angles for the 10 points
    float angles[10] = { 0, 36, 72, 108, 144, 180, 216, 252, 288, 324 };

    size = 10;

    // Convert degrees to radians
    for (int i = 0; i < 10; i++) {
        float angleRad = glm::radians(angles[i]);

        // Get random radius within min/max range
        float radius = ofRandom(min, max);

        if (radius > maxRadius) {
            maxRadius = radius;
        }

        // Compute ordered points
        float x = cx + cos(angleRad) * radius;
        float y = cy + sin(angleRad) * radius;

        switch (i) {
        case 0:  point1 = glm::vec3(x, y, 0); break;
        case 1:  point2 = glm::vec3(x, y, 0); break;
        case 2:  point3 = glm::vec3(x, y, 0); break;
        case 3:  point4 = glm::vec3(x, y, 0); break;
        case 4:  point5 = glm::vec3(x, y, 0); break;
        case 5:  point6 = glm::vec3(x, y, 0); break;
        case 6:  point7 = glm::vec3(x, y, 0); break;
        case 7:  point8 = glm::vec3(x, y, 0); break;
        case 8:  point9 = glm::vec3(x, y, 0); break;
        case 9:  point10 = glm::vec3(x, y, 0); break;
        }
    }

    radius = maxRadius;
}

void Asteroid::draw() {

	ofPushMatrix();
	ofMultMatrix(getTransform());

	ofBeginShape();
	ofVertex(point1);
	ofVertex(point2);
	ofVertex(point3);
	ofVertex(point4);
	ofVertex(point5);
	ofVertex(point6);
	ofVertex(point7);
	ofVertex(point8);
	ofVertex(point9);
	ofVertex(point10);
	ofEndShape(true);

	ofDrawLine(point1, point2);
	ofDrawLine(point2, point3);
	ofDrawLine(point3, point4);
	ofDrawLine(point4, point5);
	ofDrawLine(point5, point6);
	ofDrawLine(point6, point7);
	ofDrawLine(point7, point8);
	ofDrawLine(point8, point9);
	ofDrawLine(point9, point10);
	ofDrawLine(point10, point1);

	ofPopMatrix();
}

void Asteroid::integrate() {

    // (1) update position from velocity and time interval
    // (2) update velocity (based on acceleration
    // (3) multiply final result by the damping factor to sim drag
    // No damping because asteroids keep moving and do not stop unless destroyed

    if (ofGetFrameRate() > 0) {
        float dt = 1.0 / ofGetFrameRate();

        position += velocity * dt;

        ofVec3f accel = acceleration;
        accel += (forces * (1.0 / mass));
        velocity += accel * dt;

        forces = glm::vec3(0, 0, 0);

        // Only rotate on certain frames (to slow down asteroid spin)
        frameCounter++;
        if (frameCounter > frameSkip) {
            angle += torque;  
            frameCounter = 0;  // Reset counter
        }
    }
    
    //torque = torque * damping (no damping ==> minimal friction in vacuum of space)

}

vector<Asteroid> Asteroid::split() {
    vector<Asteroid> newAsteroids;

    if (size > 5) {  // Prevent infinite splitting
        for (int i = 0; i < 3; i++) {
            Asteroid smallAsteroid;
            smallAsteroid.position = position + glm::vec3(ofRandom(-10, 10), ofRandom(-10, 10), 0);
            smallAsteroid.size = size / 2;
            smallAsteroid.velocity = velocity + glm::vec3(ofRandom(-2, 2), ofRandom(-2, 2), 0);
            newAsteroids.push_back(smallAsteroid);
        }
    }

    return newAsteroids;
}
