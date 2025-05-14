#pragma once

#include "Projectile.h"
#include <vector>

class ProjectileManager {
public:
    ProjectileManager();
    void spawnProjectile(glm::vec3 dir);
    void update();
    void draw();
    void removeProjectile(Projectile& projectile);
    
    std::vector<Projectile> projectiles; // Vector that stores all current projectiles
    glm::vec3 position;
    float lastFireTime = 0;  // Time when last projectile was fired
    float fireRate = 0.1;     // Time (in seconds) between shots
    ofSoundPlayer fireProjectile; //  For the shot sound
};