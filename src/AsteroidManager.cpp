#include "AsteroidManager.h"
#include "ofMain.h" 

AsteroidManager::AsteroidManager() {
    difficulty = 1;
    difficultyIncreaseRate = 1.1;
    spawnAmount = 14;
}

void AsteroidManager::spawnAsteroids(int numAsteroids) {
    for (int i = 0; i < numAsteroids; i++) {
        Asteroid a;
        a.lifespan = 60;

        a.side = ofRandom(0, 4);

        float minSpd = (a.maxSpeed / 2) * difficulty;
        float maxSpd = a.maxSpeed * difficulty;

        // So you spawn on a random side
        // left
        if (a.side >= 3) {  
            a.position = glm::vec3(1, ofRandom(1, ofGetScreenHeight() - 1), 0);

            float speed = ofRandom(minSpd, maxSpd);
            a.velocity.x = (ofRandom(1) > 0.5) ? -speed : speed;  // 50% chance left or right
            a.velocity.y = ofRandom(-maxSpd, maxSpd);  // Random vertical motion
        }
        // top
        else if (a.side >= 2) {  
            a.position = glm::vec3(ofRandom(1, ofGetScreenWidth() - 1), 1, 0);
            
            float speed = ofRandom(minSpd, maxSpd);
            a.velocity.x = ofRandom(-maxSpd, maxSpd); 
            a.velocity.y = (ofRandom(1) > 0.5) ? -speed : speed; 
            
        }
        // right
        else if (a.side >= 1) {  
            a.position = glm::vec3(ofGetScreenWidth() - 1, ofRandom(1, ofGetScreenHeight() - 1), 0);

            float speed = ofRandom(minSpd, maxSpd);
            a.velocity.x = (ofRandom(1) > 0.5) ? -speed : speed;  
            a.velocity.y = ofRandom(-maxSpd, maxSpd);  
        }
        // bottom
        else { 
            a.position = glm::vec3(ofRandom(1, ofGetScreenWidth() - 1), ofGetScreenHeight() - 1, 0);
            
            float speed = ofRandom(minSpd, maxSpd);
            a.velocity.x = ofRandom(-maxSpd, maxSpd);  
            a.velocity.y = (ofRandom(1) > 0.5) ? -speed : speed;  
            
        }

        asteroids.push_back(a);
    }
}

void AsteroidManager::update() {
    for (auto& asteroid : asteroids) {
        asteroid.integrate();
    }

    float currentTime = ofGetElapsedTimef();

    for (auto it = asteroids.begin(); it != asteroids.end(); ) {

        // Checks if an asteroid is outside
        bool outside = (it->position.x < 0) || (it->position.x > ofGetWindowWidth()) || (it->position.y < 0) || (it->position.y > ofGetWindowHeight());

        // If asteroid lived longer than lifespan and is outside the window
        if ((currentTime - it->birthtime > it->lifespan) && outside) {
            it = asteroids.erase(it);
        }
        else if (outside){
            if (it->position.x < 0) {
                it->position.x = ofGetWindowWidth() - 1;
            }
            else if (it->position.x > ofGetWindowWidth()) {
                it->position.x = 1;
            }
            else if (it->position.y < 0) {
                it->position.y = ofGetWindowHeight() - 1;
            }
            else if (it->position.y > ofGetWindowHeight()) {
                it->position.y = 1;
            }

            ++it;
        } 
        else {
            ++it;
        }
    }
}

void AsteroidManager::draw() {
    for (auto& asteroid : asteroids) {
        asteroid.draw();
    }
}

void AsteroidManager::increaseDifficulty() {
    difficulty *= difficultyIncreaseRate;
}

void AsteroidManager::removeAsteroid(Asteroid& asteroid) {
    asteroids.erase(
        std::remove_if(asteroids.begin(), asteroids.end(),
            [&asteroid](const Asteroid& a) { return &a == &asteroid; }
        ),
        asteroids.end()
    );
}


int AsteroidManager::handleCollision(Asteroid& asteroid) {

    //cout << "handling collision" << endl;

    if (asteroid.size == 10) {
        // Random angle offsets for more variation
        float angle1 = ofRandom(0, TWO_PI);
        float angle2 = ofRandom(0, TWO_PI);
        float angle3 = ofRandom(0, TWO_PI);

        float minSpd = (asteroid.maxSpeed / 2) * difficulty;
        float maxSpd = asteroid.maxSpeed * difficulty;

        float newMin1 = asteroid.minRadius1 / 3;
        float newMin2 = asteroid.minRadius2 / 3;
        float newMax1 = asteroid.maxRadius1 / 3;
        float newMax2 = asteroid.maxRadius2 / 3;

        float speedScale = 0.3f; // Slower mini asteroids

        // Create three smaller asteroids on impact
        MiniAsteroid a;
        a.size = asteroid.size / 2;
        a.position = asteroid.position + glm::vec3(0, ofRandom(-10, -15), 0);
        float speed = ofRandom(minSpd, maxSpd);
        a.velocity.x = ofRandom(-maxSpd, maxSpd);
        a.velocity.y = (ofRandom(1) > 0.5) ? -speed : speed;
        a.velocity *= speedScale;

        MiniAsteroid b;
        b.size = asteroid.size / 2;
        b.position = asteroid.position + glm::vec3(ofRandom(-10, 0), ofRandom(5, 10), 0);
        b.velocity.x = (ofRandom(1) > 0.5) ? -speed : speed;
        b.velocity.y = ofRandom(-maxSpd, maxSpd);
        b.velocity *= speedScale;

        MiniAsteroid c;
        c.size = asteroid.size / 2;
        c.position = asteroid.position + glm::vec3(ofRandom(0, 10), ofRandom(5, 10), 0);
        c.velocity.x = (ofRandom(1) > 0.5) ? -speed : speed;
        c.velocity.y = ofRandom(-maxSpd, maxSpd);
        c.velocity *= speedScale;

        asteroids.push_back(a);
        asteroids.push_back(b);
        asteroids.push_back(c);

        removeAsteroid(asteroid);

        return 100; // Score for destroying a big asteroid
    }
    
    removeAsteroid(asteroid);
    return 50;  // Score for destorying a small asteroid
}