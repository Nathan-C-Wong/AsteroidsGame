#include "ProjectileManager.h"
#include "ofMain.h" 

ProjectileManager::ProjectileManager() {
    position = glm::vec3(0,0,0);
    fireProjectile.load("sounds/laser_soundfx.mp3");
    fireProjectile.setVolume(0.04f);
    fireProjectile.setMultiPlay(true);
    fireProjectile.setLoop(false);
}

void ProjectileManager::spawnProjectile(glm::vec3 dir) {
  
    float currentTime = ofGetElapsedTimef(); // Get current time

    if (currentTime - lastFireTime < fireRate) {
        return; // Prevent shooting if cooldown hasn't passed
    }

    lastFireTime = currentTime; // Update last fire time

    Projectile p;
    p.position = position;  
    p.velocity = glm::normalize(dir) * p.projSpeed;
    projectiles.push_back(p);
    fireProjectile.play();
}

void ProjectileManager::update() {
	for (auto& projectile : projectiles) {
		projectile.integrate();
	}
    float currentTime = ofGetElapsedTimef();

    for (auto it = projectiles.begin(); it != projectiles.end(); ) {

        // Checks if a projectile is outside
        bool outside = (it->position.x < 0) || (it->position.x > ofGetWindowWidth()) || (it->position.y < 0) || (it->position.y > ofGetWindowHeight());

        // Projectile expiration
        if (currentTime - it->birthtime > it->lifespan) {
            it = projectiles.erase(it);
        }
        else if (outside) {
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
void ProjectileManager::draw() {
	for (auto& projectile : projectiles) {
		projectile.draw();
	}
}

void ProjectileManager::removeProjectile(Projectile& projectile) {
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [&projectile](const Projectile& p) { return &p == &projectile; }
        ),
        projectiles.end()
    );
}

