//
//  Player.h
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#pragma once

#include "Entity.h"

class Player : public Entity {
public:
	static Sprite sprite;
	Player(World* w, double x, double y):Entity(w, &Player::sprite, x, y) {}
	
	bool update(float dt);
};