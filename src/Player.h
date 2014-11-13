//
//  Player.h
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#pragma once

#include "Entity.h"

class Player : public dgrid::Entity {
public:
	static Sprite sprite;
	Player(dgrid::World* w, double x, double y):Entity(w, &Player::sprite, x, y) {hasGravity = false;}
	
	bool update(float dt);
};