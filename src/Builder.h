//
//  Builder.h
//  PixelEngine
//
//  Created by Quinn on 11/13/14.
//
//

#pragma once

#include "Entity.h"

class Builder : public dgrid::Entity {
public:
	static Sprite sprite;
	Builder(dgrid::World* w, double x, double y):Entity(w, &Builder::sprite, x, y) {hasGravity = false;}
	
	bool update(float dt);
};