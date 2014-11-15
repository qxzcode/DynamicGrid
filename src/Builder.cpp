//
//  Builder.cpp
//  PixelEngine
//
//  Created by Quinn on 11/13/14.
//
//

#include "Builder.h"

#include <stdlib.h>

#include "World.h"
#include "TileTypes.h"

bool Builder::update(float dt) {
	dir::moveDir(sx, sy, dir, 40*dt);
	
	int mx = dgrid::util::floor(x)-2, my = dgrid::util::floor(y)-2;
	dir::moveDir(mx, my, dir::comp(dir, dir::BACKWARD), 3);
	world->setTile(1, mx + random()%5, my + random()%5, WALL);
	
	if (!(random()%200))
		dir = dir::rotateC(dir);
	if (!(random()%200))
		dir = dir::rotateCC(dir);
	
	return Unit::update(dt);
}

dgrid::Entity::Sprite Builder::sprite(1, 1);