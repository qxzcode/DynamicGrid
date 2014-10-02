//
//  Acid.h
//  PixelWorld
//
//  Created by Quinn on 9/26/14.
//
//

#pragma once

#include "Entity.h"

class Acid : public Entity {
public:
	static Sprite sprite;
	Acid(World* w, double x, double y, double sx, double sy):Entity(w, &Acid::sprite, x, y, sx, sy) {stopOnCollide=true;}
	
	bool update(float dt);
};