//
//  Acid.cpp
//  PixelWorld
//
//  Created by Quinn on 9/26/14.
//
//

#include "Acid.h"

#include "World.h"
#include "TileTypes.h"

bool Acid::update(float dt) {
	Entity::update(dt);
	if (collided) {
		world->setTile(engine::util::floor(x), engine::util::floor(y), EMPTY);
//		world->setTile(util::floor(x+1), util::floor(y), EMPTY);
//		world->setTile(util::floor(x-1), util::floor(y), EMPTY);
//		world->setTile(util::floor(x), util::floor(y+1), EMPTY);
//		world->setTile(util::floor(x), util::floor(y-1), EMPTY);
		return true;
	}
	return false;
}

engine::Entity::Sprite Acid::sprite(1, 1);