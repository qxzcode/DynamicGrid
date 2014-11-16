//
//  Tank.h
//  PixelEngine
//
//  Created by Quinn on 11/15/14.
//
//

#pragma once

#include "Unit.h"
#include "direction.h"

#include <stdlib.h>

class Tank : public Unit {
public:
	static Sprite sprite;
	Tank(dgrid::World* w, double x, double y):Unit(w, &Tank::sprite, x, y, 10),dir(random()%4) {}
	
	bool update(float dt);
	
	dir::dir dir;
};