//
//  Builder.h
//  PixelEngine
//
//  Created by Quinn on 11/13/14.
//
//

#pragma once

#include "Unit.h"
#include "direction.h"

#include <stdlib.h>

class Builder : public Unit {
public:
	static Sprite sprite;
	Builder(dgrid::World* w, double x, double y):Unit(w, &Builder::sprite, x, y, 10),dir(random()%4) {}
	
	bool update(float dt);
	
	dir::dir dir;
};