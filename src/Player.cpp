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
	if (input::left)  sx -= 200*dt;
	if (input::right) sx += 200*dt;
	if (input::up)  sy -= 200*dt;
	if (input::down) sy += 200*dt;
	
	Entity::update(dt);
	return false;
}

dgrid::Entity::Sprite Player::sprite(1, 1);