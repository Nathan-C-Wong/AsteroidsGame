#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//Gui Setup
	ofSetVerticalSync(true);
	
	//Borderless Fullscreen
	ofSetWindowShape(ofGetScreenWidth(), ofGetScreenHeight());
	ofSetWindowPosition(0, 0);
	
	headingToggle = false;

	// Sounds
	projectileCollision.load("sounds/projectile_explosion.mp3");  // Missile explosion
	projectileCollision.setVolume(0.05f);
	projectileCollision.setMultiPlay(true);
	projectileCollision.setLoop(false);

	playerCollision.load("sounds/pixel_ouch.mp3");  // Player collision
	playerCollision.setVolume(0.75f);
	playerCollision.setMultiPlay(true);
	playerCollision.setLoop(false);

	playerThrust.load("sounds/rocket_thruster1.wav");
	playerThrust.setVolume(0.90f);
	playerThrust.setMultiPlay(false);
	playerThrust.setLoop(false);

	clickSound.load("sounds/btn-click.mp3");
	clickSound.setVolume(0.90f);
	clickSound.setMultiPlay(false);
	clickSound.setLoop(false);

	// When clicking nightmare mode
	nightmareSound.load("sounds/nightmareSound.mp3");
	nightmareSound.setVolume(0.70f);
	nightmareSound.setMultiPlay(false);
	nightmareSound.setLoop(false);

	// Soundtracks
	spaceCalmTrack.load("sounds/calm-space-music.mp3");
	spaceCalmTrack.setVolume(0.20f);
	spaceCalmTrack.setMultiPlay(false);
	spaceCalmTrack.setLoop(true);

	nightmareTrack1.load("sounds/nightmare_track1.mp3");
	nightmareTrack1.setVolume(0.20f);
	nightmareTrack1.setMultiPlay(false);
	nightmareTrack1.setLoop(true);

	nightmareTrack2.load("sounds/nightmare_track2.mp3");
	nightmareTrack2.setVolume(0.20f);
	nightmareTrack2.setMultiPlay(false);
	nightmareTrack2.setLoop(true);

	// Mouse set-up
	clickedDown = false;

	// Player setup
	player.position = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0.0);
	player.initializeTriangle(60, 40);
	//player.quickness = speed;

	player.angle = 0;

	// Background Image
	if (!background.load("images/Starry_Sky.png")) {
		cout << "can't load background" << endl;
		ofExit(0);
	}

	// Game attribute setup
	scoreFont.load("fonts/PressStart2P-Regular.ttf", 24);
	score = 0;
	highscoreFont.load("fonts/PressStart2P-Regular.ttf", 15);
	highScore = 0;

	// Projectile Manager 
	pm.position = player.vertexC;

	// Game set-up
	mode = "Choosing";
	livesFont.load("fonts/PressStart2P-Regular.ttf", 14);
	lives = 10;
	invincible = false;
	invincibleFont.load("fonts/PressStart2P-Regular.ttf", 14);

	// Menu
	menuTitle.load("fonts/PressStart2P-Regular.ttf", 100);

	easyButton.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2 - 150, 200, 100);
	easyFont.load("fonts/PressStart2P-Regular.ttf", 20);

	mediumButton.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2, 200, 100);
	mediumFont.load("fonts/PressStart2P-Regular.ttf", 20);

	hardButton.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 150, 200, 100);
	hardFont.load("fonts/PressStart2P-Regular.ttf", 20);

	nightmareButton.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 300, 200, 100);
	nightmareFont.load("fonts/PressStart2P-Regular.ttf", 15);

	menu = true;
	gameStart = false;

	// Game Over
	finalScore.load("fonts/PressStart2P-Regular.ttf", 100);
	replayButton.set(ofGetWidth() / 2 - 100, ofGetHeight() / 2 + 150, 200, 100);
	replayFont.load("fonts/PressStart2P-Regular.ttf", 20);
}


//--------------------------------------------------------------
void ofApp::update() {

	if (gameStart) {
		if (lives < 1) {
			invincible = true;
			gameOver = true;
			gameStart = false;

			if (spaceCalmTrack.isPlaying()) {
				spaceCalmTrack.stop();
			}
			if (nightmareTrack1.isPlaying()) {
				nightmareTrack1.stop();
			}
			if (nightmareTrack2.isPlaying()) {
				nightmareTrack2.stop();
			}
			
			mode = "Choosing";
		}

		// Grants 2 seconds of invincibility on respawn
		if ((ofGetElapsedTimef() - invincibleTimer > 2) && !gameOver) {
			invincible = false;
		}

		ofSoundUpdate();

		// Player Physics Integration
		player.integrate();

		// Explosion update
		float dt = 1.0f / ofGetFrameRate();
		particleSystem.update(dt);

		// Keymap
		if (keymap[OF_KEY_UP] || keymap['W'] || keymap['w']) {
			particleSystem.thrusterParticle(player.position, -player.heading() * player.quickness * 1.5);
			player.moveForward();
			isThrusting = true;
		}
		if (keymap[OF_KEY_DOWN] || keymap['S'] || keymap['s']) {
			particleSystem.backwardsThrusterParticle(player.position, -player.heading() * player.quickness * 1.5);
			player.moveBackwards();
			isThrusting = true;
		}
		if (keymap[OF_KEY_RIGHT] || keymap['D'] || keymap['d']) {
			player.turnRight();
		}
		if (keymap[OF_KEY_LEFT] || keymap['A'] || keymap['a']) {
			player.turnLeft();
		}
		if (keymap[' ']) {
			pm.spawnProjectile(player.heading());

		}
		if (clickedDown == true) {
			pm.spawnProjectile(player.heading());
		}

		// Screen Wrapping
		if (player.position.x <= 0) {
			player.position = glm::vec3(ofGetWindowWidth() - 10, player.position.y, 0);
		}
		if (player.position.x >= ofGetWindowWidth()) {
			player.position = glm::vec3(0 + 10, player.position.y, 0);
		}
		if (player.position.y <= 0) {
			player.position = glm::vec3(player.position.x, ofGetWindowHeight() - 10, 0);
		}
		if (player.position.y >= ofGetWindowHeight()) {
			player.position = glm::vec3(player.position.x, 0 + 10, 0);
		}

		// Spawn asteroids if less than threshold amount of asteroids on screen
		// AND make sure 3 seconds has passed before spawning from gameStart (prevents player from instantly dying)
		currentTime = ofGetElapsedTimef();
		if ((am.asteroids.size() < (am.spawnAmount / 2)) && (currentTime - gameStartTime > 3)) {
			am.spawnAsteroids(am.spawnAmount);
			am.increaseDifficulty();
		}

		// Asteroid Manager
		am.update();

		// Projectile Manager
		pm.position = player.position;
		pm.update();

		// Thrust Sound
		if (isThrusting) {
			if (!playerThrust.isPlaying()) {  // Prevent restarting every frame
				//playerThrust.setLoop(true);
				playerThrust.play();
			}
		}
		else {
			//playerThrust.setLoop(false);  // Stop looping when not moving
			playerThrust.stop();  // Stop sound when not moving
		}

		for (auto& asteroid1 : am.asteroids) {
			for (auto& asteroid2 : am.asteroids) {
				if (checkAsteroidCollision(asteroid1, asteroid2)) {
					// Handle collision
					glm::vec3 contactPoint = getContactPoint(asteroid1, asteroid2);
					particleSystem.addExplosion(contactPoint, 10);
					

					// bounce-back
					glm::vec3 direction = glm::normalize(asteroid1.position - asteroid2.position);
					float overlap = (asteroid1.radius + asteroid2.radius) - glm::distance(asteroid1.position, asteroid2.position);

					// Push each asteroid away from each other
					asteroid1.position += direction * (overlap / 2.0f);
					asteroid2.position -= direction * (overlap / 2.0f);

					std::swap(asteroid1.velocity, asteroid2.velocity);
				}
			}
		}

		// Handle collision between projectile and asteroid
		for (auto& projectile : pm.projectiles) {
			for (auto& asteroid : am.asteroids) {

				if (checkCollision(asteroid, projectile)) {
					glm::vec3 astPos = asteroid.position;
					projectileCollision.play();
					pm.removeProjectile(projectile);
					float destructionScore = am.handleCollision(asteroid);  // Handles asteroid collision
					score += destructionScore;
					if (destructionScore < 100) {
						particleSystem.addSmallExplosion(astPos, 80);
					}
					else {
						particleSystem.addExplosion(astPos, 350);
					}
					
				}
			}
		}

		// When game first starts player becomes invincible
		if (!invincible) {
			// Handle crash between player and asteroid
			for (auto& asteroid : am.asteroids) {
				if (checkCrash(player, asteroid)) {
					playerCollision.play();
					glm::vec3 astPos = asteroid.position;
					float destructionScore = am.handleCollision(asteroid);  // Handles asteroid collision
					score += destructionScore;
					if (destructionScore < 100) {
						particleSystem.addSmallExplosion(astPos, 80);
					}
					else {
						particleSystem.addExplosion(astPos, 350);
					}

					particleSystem.addPlayerExplosion(player.position);
					player.position = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0.0);
					--lives;  // May need to add delay to this
					invincible = true;
					// Invincibility on respawn
					invincibleTimer = ofGetElapsedTimef();

					player.angle = 0;
					player.velocity = glm::vec3(0, 0, 0);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	
	// Draw background
	background.draw(0, 0, ofGetWidth(), ofGetHeight());

	if (menu) {

		// Difficulties
		ofSetColor(37, 224, 20);  // Green
		ofDrawRectangle(easyButton);

		ofSetColor(217, 232, 9);  // Yellow
		ofDrawRectangle(mediumButton);
		
		ofSetColor(245, 10, 10); // Red
		ofDrawRectangle(hardButton);
		
		ofSetColor(72, 5, 97); // Purple
		ofDrawRectangle(nightmareButton);

		ofSetColor(255);

		// Game title
		string heading = "Asteroids";
		float textWidth = menuTitle.stringWidth(heading);
		float xPos = (ofGetWidth() / 2) - (textWidth / 2);
		menuTitle.drawString(heading, xPos, 300);

		// Draw the button text (easy button)
		ofSetColor(255);
		string easyTxt = "Easy";
		easyFont.drawString(easyTxt, easyButton.getX() + 48, easyButton.getY() + 67);

		string mediumTxt = "Medium";
		mediumFont.drawString(mediumTxt, mediumButton.getX() + 20, mediumButton.getY() + 67);

		string hardTxt = "Hard";
		hardFont.drawString(hardTxt, hardButton.getX() + 48, hardButton.getY() + 67);

		string nightmareTxt = "Nightmare";
		nightmareFont.drawString(nightmareTxt, nightmareButton.getX() + 8, nightmareButton.getY() + 65);

		ofTrueTypeFont highScoreHeader;
		highScoreHeader.load("fonts/PressStart2P-Regular.ttf", 20);
		highScoreHeader.drawString("High Score", ofGetWidth() / 2 - 130, ofGetHeight() / 2 - 350);

		string highscoreText = ofToString(highScore);
		float hsTextWidth = highscoreFont.stringWidth(highscoreText);
		float hsxPos = (ofGetWidth() / 2) - (hsTextWidth / 2);
		scoreFont.drawString(highscoreText, hsxPos, ofGetHeight() / 2 - 300);

	}

	if (gameStart) {

		// Invincibility Circle
		if (invincible) {
			drawForceField(player.angle, player.position.x, player.position.y);
		}

		// Draw player
		player.draw();
		
		particleSystem.draw();
		ofSetColor(255);

		// Draw score
		string scoreText = ofToString(score);
		float textWidth = scoreFont.stringWidth(scoreText);
		float xPos = (ofGetWidth() / 2) - (textWidth / 2);
		scoreFont.drawString(scoreText, xPos, 60);

		// Draw lives
		string livesText = "Lives: " + ofToString(lives);
		livesFont.drawString(livesText, 20, 40);

		// Statuses
		string invinStr = "";
		if (invincible) {
			invinStr = "True";
		}
		else {
			invinStr = "False";
		}

		// Draw FPS
		string frameText = "FPS: " + ofToString(ofGetFrameRate());
		ofDrawBitmapString(frameText, ofGetWidth() - 120, 20);

		am.draw();
		pm.draw();
	}
	
	if (gameOver && !gameStart) {
		ofSetColor(100, 100, 255);  // Blue
		ofDrawRectangle(replayButton);

		ofSetColor(255);
		string finalScoreStr = ofToString(score);
		float textWidth = menuTitle.stringWidth(finalScoreStr);
		float xPos = (ofGetWidth() / 2) - (textWidth / 2);
		finalScore.drawString(finalScoreStr, xPos, 300);
		invincibleTimer = ofGetElapsedTimef();

		// Draw the button text
		ofSetColor(255);
		string replayTxt = "Menu";
		replayFont.drawString(replayTxt, replayButton.getX() + 48, replayButton.getY() + 67);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (isalpha(key)) {  // Only apply tolower() for letters
		keymap[tolower(key)] = true;
	}
	else {
		keymap[key] = true;
	}

	switch (key) {
		case 'f':  //Full Screen Button
			ofToggleFullscreen();
			break;
		default:
			break;
		}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (isalpha(key)) {  // Only apply tolower() for letters
		keymap[tolower(key)] = false;
	}
	else {
		keymap[key] = false;
	}

	if (!keymap[OF_KEY_UP] || !keymap['W'] || !keymap['w']) {
		isThrusting = false;
	}
	if (!keymap[OF_KEY_DOWN] || !keymap['S'] || !keymap['s']) {
		isThrusting = false;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	if (gameStart) {
		clickedDown = true;
	}

	if (menu) {
		if (easyButton.inside(x, y) && !gameStart) {
			menu = false;
			gameStart = true;  // Start the game
			gameStartTime = ofGetElapsedTimef();
			mode = "Easy";
			clickSound.play();
			am.spawnAmount = 14;

			spaceCalmTrack.play();
		}
		else if (mediumButton.inside(x, y) && !gameStart) {
			menu = false;
			gameStart = true;  // Start the game
			gameStartTime = ofGetElapsedTimef();
			am.difficulty = 2;
			mode = "Medium";
			clickSound.play();
			am.spawnAmount = 14;

			spaceCalmTrack.play();
		}
		else if (hardButton.inside(x, y) && !gameStart) {
			menu = false;
			gameStart = true;  // Start the game
			gameStartTime = ofGetElapsedTimef();
			am.difficulty = 3;
			mode = "Hard";
			clickSound.play();
			am.spawnAmount = 14;

			spaceCalmTrack.play();
		}
		else if (nightmareButton.inside(x, y) && !gameStart) {
			menu = false;
			gameStart = true;  // Start the game
			gameStartTime = ofGetElapsedTimef();
			am.difficulty = 5;
			mode = "Good Luck...";
			nightmareSound.play();
			am.spawnAmount = 20;

			float randomSong = ofRandom(1);
			
			if (randomSong > 0.5) {
				nightmareTrack1.play();
			}
			else {
				nightmareTrack2.play();
			}
			
		}
	}

	if (gameOver) {
		if (replayButton.inside(x, y) && !gameStart) {
			menu = true;
			gameStart = false;  // Start the game
			gameOver = false;

			// Reset
			if (score > highScore) {
				highScore = score;
			}
			score = 0;
			lives = 10;
			player.angle = 0;
			player.velocity = glm::vec3(0, 0, 0);
			invincible = false;
			am.asteroids.clear();
			am.difficulty = 1;
			mode = "Choosing";
		}
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	clickedDown = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
