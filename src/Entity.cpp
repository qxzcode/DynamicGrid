//
//  Entity.cpp
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#include "Entity.h"

#include <math.h>

#include "World.h"
#include "util.h"
#include "TileTypes.h"

using namespace dgrid;

bool Entity::update(float dt) {
	// handle physics stuff
	if (hasGravity)
		sy += 400*dt;
	x += sx*dt;
	y += sy*dt;
	
	// friction
	double f = 1 + (friction*dt);
	sx /= f;
	sy /= f;
	
	doCollide();
	
	return false;
}

void Entity::draw() {
	if (drawn) {
		clearSprite(lastDrawX, lastDrawY);
	}
	int mx = util::floor(x), my = util::floor(y);
	drawSprite(mx, my);
	
	lastDrawX = mx;
	lastDrawY = my;
	drawn = true;
}

void Entity::drawSprite(int mx, int my) {
	for (int lx = 0; lx < curSprite->w; lx++) {
		int wx = mx+lx;
		for (int ly = 0; ly < curSprite->h; ly++) {
			int wy = my+ly;
			world->setTile(2, wx, wy, curSprite->tile(lx, ly));
		}
	}
}

void Entity::clearSprite(int mx, int my) {
	for (int lx = 0; lx < curSprite->w; lx++) {
		int wx = mx+lx;
		for (int ly = 0; ly < curSprite->h; ly++) {
			int wy = my+ly;
			world->setTile(2, wx, wy, 0);
		}
	}
}

void Entity::onRemove() {
	if (drawn) {
		clearSprite(lastDrawX, lastDrawY);
	}
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
			if (world->tileTypes[world->getTile(1, wx, wy)].solid && curSprite->solid(lx, ly)) {
				if (cx) *cx = lx;
				if (cy) *cy = ly;
				return true;
			}
		}
	}
	return false;
}


#define SPRITE_INIT(W,H) w(W),h(H),solidData(new bool[W*H]),tileData(new tileID[W*H])
Entity::Sprite::Sprite(int w, int h):SPRITE_INIT(w,h) {
	for (int i = 0; i < w*h; i++) {
		solidData[i] = true;
		tileData[i] = ACID;
	}
}
Entity::Sprite::Sprite(const Sprite& s):SPRITE_INIT(s.w,s.h) {
	for (int i = 0; i < w*h; i++) {
		solidData[i] = s.solidData[i];
		tileData[i] = s.tileData[i];
	}
}
#undef SPRITE_INIT
Entity::Sprite::~Sprite() {
	delete [] solidData;
	delete [] tileData;
}





