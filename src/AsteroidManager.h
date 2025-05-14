#pragma once

#include "Asteroid.h"
#include <vector>

class AsteroidManager {
public:
    AsteroidManager();
    void spawnAsteroids(int numAsteroids);
    void update();
    void draw();
    void increaseDifficulty();
    void removeAsteroid(Asteroid& asteroid);
    int handleCollision(Asteroid& asteroid); // Handles splitting when hit

    std::vector<Asteroid> asteroids; // Vector that stores all current asteroids
    float difficulty;
    float difficultyIncreaseRate;
    int spawnAmount;
};
