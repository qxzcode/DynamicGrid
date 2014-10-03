//
//  Entity.cpp
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#include "Entity.h"

#include "World.h"
#include "util.h"
#include "TileTypes.h"

bool Entity::update(float dt) {
	// handle physics stuff
	if (hasGravity)
		sy += 400*dt;
	x += sx*dt;
	y += sy*dt;
	
	double f = 1 + (2.0*dt);
	sx /= f;
	sy /= f;
	
	doCollide();
	
	return false;
}

// temporary #includes
#include "worldFormat.h"
#include "cinder/gl/gl.h"
using namespace ci;

void Entity::draw() {
	glDisable(GL_TEXTURE_2D);
	gl::color(Color(1, 0, 0));
	float x = util::floor(this->x)*PIXEL_SIZE, y = util::floor(this->y)*PIXEL_SIZE;
	gl::drawSolidRect(Rectf(x, y, x+PIXEL_SIZE, y+PIXEL_SIZE));
}

void Entity::doCollide() {
	double tdx = x-lastX, tdy = y-lastY;
	double atdx = fabsf(tdx), atdy = fabsf(tdy);
	if (atdx<1.0f && atdy<1.0f) {
		collided = doCollideStep();
	} else {
		int steps = util::ceil(atdx>atdy?atdx:atdy);
		double dx = tdx/steps, dy = tdy/steps;
		x = lastX; y = lastY;
		collided = false;
		for (int n = 0; n < steps; n++) {
			x += dx; y += dy;
			collided |= doCollideStep();
			if (collided && stopOnCollide)
				break;
		}
	}
}

bool Entity::doCollideStep() {
	bool collided = onGround = false;
	if (isColliding(lastX, lastY)) {
		// collision glitch :( - ignore collisions
//		std::cout << "Collision glitch!!!! :(((((\n";
	} else {
		
		static const double E = 0.001;
		
		// collide on X axis
		if (isColliding(x, lastY)) {
			if (!stopOnCollide) {
				x =	x-lastX > 0?
					double(util::floor(x))-E :
					double(util::ceil(x))+E;
				sx = 0;
			}
			collided = true;
		}
		// collide on Y axis
		if (isColliding(x, y)) {
			if (!stopOnCollide) {
				y =	y-lastY > 0?
					(onGround=true,double(util::floor(y))-E) :
					double(util::ceil(y))+E;
				sy = 0;
			}
			collided = true;
		}
		
	}
	
	lastX = x; lastY = y;
	return collided;
}

bool Entity::isColliding(double x, double y, int* cx, int* cy) {
	int mx = util::floor(x), my = util::floor(y);
	for (int lx = 0; lx < curSprite->w; lx++) {
		int wx = mx+lx;
		for (int ly = 0; ly < curSprite->h; ly++) {
			int wy = my+ly;
			if (TILE_SOLID[world->getTile(wx, wy)] && curSprite->solid(lx, ly)) {
				if (cx) *cx = lx;
				if (cy) *cy = ly;
				return true;
			}
		}
	}
	return false;
}