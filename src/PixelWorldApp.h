//
//  PixelWorldApp.h
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"

#include "World.h"
#include "PixelWorldGen.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PixelWorldApp : public AppNative {
public:
	PixelWorldApp();
	
	void prepareSettings(Settings* settings);
	void setup();
	void shutdown();
	
	void resize();
	void keyDown(KeyEvent event);
	void keyUp(KeyEvent event);
	void key(int code, bool down);
	
	void update();
	void draw();
	
	dgrid::World world;
	PixelWorldGen generator;
	
protected:
	float getDelta() {
		double curTime = getElapsedSeconds();
		double delta = curTime-lastTime;
		lastTime = curTime;
		return float(delta);
	}
	double lastTime;
	float timer = 0;
	int frameCount = 0;
	
};