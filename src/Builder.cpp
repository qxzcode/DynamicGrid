//
//  Builder.cpp
//  PixelEngine
//
//  Created by Quinn on 11/13/14.
//
//

#include "Builder.h"

#include "World.h"
#include "TileTypes.h"

bool Builder::update(float dt) {
	sx += 200*dt;
	
	int mx = dgrid::util::floor(x), my = dgrid::util::floor(y);
	for (int lx = 0; lx < 3; lx++) {
		int wx = mx+lx;
		for (int ly = 0; ly < 3; ly++) {
			int wy = my+ly;
			world->setTile(1, wx, wy, WALL);
		}
	}
	
	Entity::update(dt);
	return false;
}

dgrid::Entity::Sprite Builder::sprite(1, 1);