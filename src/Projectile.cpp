#include "Projectile.h"

Projectile::Projectile() {
	birthtime = ofGetElapsedTimef();
    lifespan = 2;
    projSpeed = 1000;
    mass = 1.0f;
}

void Projectile::draw() {
    ofDrawSphere(position, 4);
}

void Projectile::integrate() {
    if (ofGetFrameRate() > 0) {
        float dt = 1.0 / ofGetFrameRate();

        position += velocity * dt;

    }
}