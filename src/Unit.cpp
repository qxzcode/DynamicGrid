//
//  Unit.cpp
//  PixelEngine
//
//  Created by Quinn on 11/14/14.
//
//

#include "Unit.h"

#include "util.h"
#include "World.h"
#include "TileTypes.h"

bool Unit::update(float dt) {
	bool remove = false;
	
	if (health > maxHealth) health = maxHealth;
	if (health <= 0) {
		// unit was destroyed... drop scrap?
		int mx = dgrid::util::floor(x), my = dgrid::util::floor(y);
		for (int lx = 0; lx < curSprite->w; lx++) {
			int wx = mx+lx;
			for (int ly = 0; ly < curSprite->h; ly++) {
				int wy = my+ly;
				world->setTile(1, wx, wy, SCRAP);
			}
		}
		remove = true;
	}
	
	return Entity::update(dt) || remove;
}
