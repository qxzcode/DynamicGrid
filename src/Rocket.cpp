//
//  Rocket.cpp
//  PixelEngine
//
//  Created by Quinn on 11/15/14.
//
//

#include "Rocket.h"

#include "World.h"
#include "TileTypes.h"

bool Rocket::update(float dt) {
	int mx = dgrid::util::floor(x), my = dgrid::util::floor(y);
	if (collided || (timer+=dt)>1.0) {
		for (int tx = mx-3; tx <= mx+3; tx++)
			for (int ty = my-3; ty <= my+3; ty++)
				world->setTile(1, tx, ty, 0);
		return true;
	}
	
	return Entity::update(dt);
}

dgrid::Entity::Sprite Rocket::sprite(1, 1);