//
//  PixelWorldApp.cpp
//  PixelWorld
//
//  Created by Quinn on 9/11/14.
//
//

#include "PixelWorldApp.h"

#include "input.h"

PixelWorldApp* appInstance;

void PixelWorldApp::prepareSettings(Settings* settings) {
	settings->setWindowSize(1000, 800);
}

void PixelWorldApp::setup() {
	getWindow()->setTitle("Pixel World");
	world.init();
	getDelta();
}

void PixelWorldApp::shutdown() {
	
}


void PixelWorldApp::resize() {
	
}

void PixelWorldApp::keyDown(KeyEvent event) {
	key(event.getCode(), true);
}

void PixelWorldApp::keyUp(KeyEvent event) {
	key(event.getCode(), false);
}

void PixelWorldApp::key(int code, bool down) {
	switch (code) {
		case KeyEvent::KEY_a:
			leftDown = down;
			break;
		case KeyEvent::KEY_d:
			rightDown = down;
			break;
		case KeyEvent::KEY_SPACE:
			jumpDown = down;
			break;
	}
}


void PixelWorldApp::update() {
	// update FPS counter
	float dt = getDelta();
	timer += dt;
	frameCount++;
	if (timer > 1.0) {
		char str[50];
		sprintf(str, "Pixel World (%i FPS)", frameCount);
		getWindow()->setTitle(str);
		timer -= 1.0;
		frameCount = 0;
	}
	
	// update game
	static const float DT_MAX = 1.0f/30.0f; // cap dt to prevent glitches in cases of extreme lag (such as when the world first loads)
	world.update(dt>DT_MAX? DT_MAX : dt);
}

void PixelWorldApp::draw() {
	world.draw(getWindowWidth(), getWindowHeight());
}




CINDER_APP_NATIVE( PixelWorldApp, RendererGl(RendererGl::AA_NONE) )
