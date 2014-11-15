//
//  Unit.cpp
//  PixelEngine
//
//  Created by Quinn on 11/14/14.
//
//

#include "Unit.h"

bool Unit::update(float dt) {
	bool remove = false;
	
	if (health > maxHealth) health = maxHealth;
	if (health <= 0) {
		// unit was destroyed... drop scrap?
		remove = true;
	}
	
	return Entity::update(dt) || remove;
}
