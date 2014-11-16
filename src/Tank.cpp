//
//  Tank.cpp
//  PixelEngine
//
//  Created by Quinn on 11/15/14.
//
//

#include "Tank.h"

#include <stdlib.h>
#include <math.h>

#include "World.h"
#include "TileTypes.h"
#include "Rocket.h"

bool Tank::update(float dt) {
	if (!(random()%30)) {
		double angle = double(random()%360)*(M_PI/180.0);
		world->spawnEntity(new Rocket(world, x, y, cos(angle)*100, sin(angle)*100));
	}
	
	return Unit::update(dt);
}

dgrid::Entity::Sprite Tank::sprite(2, 2);