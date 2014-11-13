//
//  Player.cpp
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#include "Player.h"

#include "World.h"
#include "input.h"
#include "TileTypes.h"

bool Player::update(float dt) {
	if (input::left)  sx -= 200*dt;
	if (input::right) sx += 200*dt;
	if (input::up)  sy -= 200*dt;
	if (input::down) sy += 200*dt;
	
	int mx = dgrid::util::floor(x), my = dgrid::util::floor(y);
	for (int lx = 0; lx < 3; lx++) {
		int wx = mx+lx;
		for (int ly = 0; ly < 3; ly++) {
			int wy = my+ly;
			if (world->getTile(1, wx, wy) == GRASS) {
				world->setTile(1, wx, wy, EMPTY);
			}
		}
	}
	
	Entity::update(dt);
	return false;
}

dgrid::Entity::Sprite Player::sprite(1, 1);