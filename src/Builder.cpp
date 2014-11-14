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
	sx += 40*dt;
	
	int mx = dgrid::util::floor(x), my = dgrid::util::floor(y);
	world->setTile(1, mx - 1 - random()%10, my - 10 + random()%20, WALL);
	
	Entity::update(dt);
	return false;
}

dgrid::Entity::Sprite Builder::sprite(1, 1);