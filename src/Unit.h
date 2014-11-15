//
//  Unit.h
//  PixelEngine
//
//  Created by Quinn on 11/14/14.
//
//

#pragma once

#include "Entity.h"

class Unit : public dgrid::Entity {
public:
	Unit(dgrid::World* w, Entity::Sprite* sprite, double x, double y, int maxH):Entity(w, sprite, x, y),maxHealth(maxH),health(maxH) {
		hasGravity = false;
	}
	
	bool update(float dt);
	
	int health, maxHealth;
};