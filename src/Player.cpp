//
//  Player.cpp
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#include "Player.h"

#include "input.h"
#include "World.h"

bool Player::update(float dt) {
	if (leftDown)  sx -= 200*dt;
	if (rightDown) sx += 200*dt;
	if (jumpDown) {
		if (onGround)
			sy = -250;
		for (int n = 0; n < 50; n++)
			world->spawnParticle(int(x), int(y));
	}
	
	Entity::update(dt);
	return false;
}

dgrid::Entity::Sprite Player::sprite(1, 1);