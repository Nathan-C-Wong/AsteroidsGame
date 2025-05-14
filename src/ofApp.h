#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Shape.h"
#include "TriangleShape.h"
#include "DynamicShape.h"
#include "Asteroid.h"
#include "AsteroidManager.h"
#include "Projectile.h"
#include "ProjectileManager.h"
#include "Particle.h"
#include "ParticleSystem.h"

// Projectile, Asteroid collision
bool checkCollision(Asteroid& a, Projectile& p) {
	glm::vec3 pPos = p.position; // projectile position
	glm::vec3 aPos = a.position; // asteroid position

	float distance = glm::length(pPos - aPos);  // Compute distance
	float asteroidRadius = a.radius;  // Assuming Asteroid has a radius

	return distance <= asteroidRadius;
}

// Asteroid, Asteroid collision
bool checkAsteroidCollision(Asteroid& a, Asteroid& b) {
	if (&a == &b) return false;  // Skip if it's the same asteroid

	float distance = glm::distance(a.position, b.position);
	float combinedRadius = a.radius + b.radius;

	return distance < combinedRadius;
}

// Get contact point of asteroids
glm::vec3 getContactPoint(Asteroid& a, Asteroid& b) {
	glm::vec3 dir = b.position - a.position;
	float distance = glm::length(dir);

	if (distance == 0) {
		// Avoid division by zero — return midpoint if they're on top of each other
		return a.position;
	}

	glm::vec3 dirNormalized = glm::normalize(dir);
	glm::vec3 contactPoint = a.position + dirNormalized * a.radius;

	return contactPoint;
}


// Point Based Collision Detection
bool checkCrash(TriangleShape& t, Asteroid& a) {
	glm::vec3 pPos = t.position; // player position
	glm::vec3 aPos = a.position; // asteroid position

	// Every triangle vertex
	glm::vec3 v1 = t.position + t.vertexC;  // Tip
	glm::vec3 v2 = t.position + t.vertexA;  // Bottom left
	glm::vec3 v3 = t.position + t.vertexB;  // Bottom right

	// Midpoints between vertices
	glm::vec3 m1 = (v1 + v2) / 2;  // Middle left
	glm::vec3 m2 = (v1 + v3) / 2;  // Middle Right
	glm::vec3 m3 = (v2 + v3) / 2;  // Middle bottom

	// Center positions 
	float distance = glm::length(pPos - aPos);  // Compute distance
	float asteroidRadius = a.radius;  // Assuming Asteroid has a radius
	
	// Tip vertex to center
	float tDistance = glm::length(v1 - aPos);
	if (tDistance <= asteroidRadius) {
		return true;
	}

	// Bottom left vertex to center
	float blDistance = glm::length(v2 - aPos);
	if (blDistance <= asteroidRadius) {
		return true;
	}

	// Bottom right vertex to center
	float brDistance = glm::length(v3 - aPos);
	if (brDistance <= asteroidRadius) {
		return true;
	}

	// Added middle vertices for more precision (e.g. side hits)

	// Middle left vertex to center
	float mlDistance = glm::length(m1 - aPos);
	if (mlDistance <= asteroidRadius) {
		return true;
	}

	// Middle right vertex to center
	float mrDistance = glm::length(m2 - aPos);
	if (mrDistance <= asteroidRadius) {
		return true;
	}

	// Middle bottom vertex to center
	float botDistance = glm::length(m3 - aPos);
	if (botDistance <= asteroidRadius) {
		return true;
	}

	return distance <= asteroidRadius;
}

void drawForceField(float angle, float x, float y) {
	ofPushMatrix();
	ofTranslate(x, y);
	ofRotateDeg(angle); 

	ofSetColor(ofColor::aqua);
	ofNoFill();  
	ofDrawEllipse(0, 0, 70, 110); 
	ofFill();
	ofSetColor(255);

	ofPopMatrix();
}

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		bool bHide;
		bool headingToggle;

		// Player object
		TriangleShape player;

		// Image and Sliders
		ofxFloatSlider speed;
		ofxToggle drawHeading;
		ofxFloatSlider triangleScale;
		ofxPanel gui;

		// Sounds
		ofSoundPlayer playerThrust;
		ofSoundPlayer  projectileCollision;
		ofSoundPlayer  playerCollision;
		bool isThrusting = false;

		// Background image
		ofImage background;

		// Drag and Shape variables
		bool clickedDown;
		glm::vec3 mousePosition;
		glm::vec3 vectorFromMiddle;

		// Keymap
		map<int, bool> keymap;
		bool movingForward; 
		bool movingBackward;
		bool movingLeft;
		bool movingRight;

		// Player Attributes
		ofTrueTypeFont scoreFont;
		ofTrueTypeFont highscoreFont;
		int score;
		int highScore;

		// Asteroid Spawner
		AsteroidManager am;

		// Particle Explosion
		ParticleSystem particleSystem;

		// Projectile Test
		ProjectileManager pm;

		// Game attributes
		ofTrueTypeFont livesFont;
		int lives;

		// Statuses
		ofTrueTypeFont invincibleFont;

		// Game setup
		bool menu;
		bool gameStart;
		bool endScreen;
		string mode;

		float gameStartTime;
		float currentTime;
		bool invincible;
		float invincibleTimer;
		bool gameOver;
		
		// Menu Screen
		ofSoundPlayer clickSound;
		ofSoundPlayer nightmareSound;

		// Sound tracks
		ofSoundPlayer spaceCalmTrack;
		ofSoundPlayer nightmareTrack1;
		ofSoundPlayer nightmareTrack2;

		ofTrueTypeFont menuTitle;

		ofRectangle easyButton;
		ofRectangle mediumButton;
		ofRectangle hardButton;
		ofRectangle nightmareButton;

		ofTrueTypeFont easyFont;
		ofTrueTypeFont mediumFont;
		ofTrueTypeFont hardFont;
		ofTrueTypeFont nightmareFont;

		// Game Over Screen
		ofTrueTypeFont finalScore;
		ofRectangle replayButton;
		ofTrueTypeFont replayFont;
};
