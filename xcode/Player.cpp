//
//  Player.cpp
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#include "Player.h"

#include "PixelWorldApp.h"

bool Player::update(float dt) {
	if (appInstance->leftDown)  sx -= 200*dt;
	if (appInstance->rightDown) sx += 200*dt;
	if (appInstance->jumpDown) {
		if (onGround)
			sy = -250;
		for (int n = 0; n < 60; n++)
			world->spawnParticle(int(x), int(y));
	}
	
	Entity::update(dt);
	return false;
}

Entity::Sprite Player::sprite(1, 1);