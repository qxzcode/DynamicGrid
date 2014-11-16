//
//  Rocket.h
//  PixelEngine
//
//  Created by Quinn on 11/15/14.
//
//

#include "Entity.h"

class Rocket : public dgrid::Entity {
public:
	static Sprite sprite;
	Rocket(dgrid::World* w, double x, double y, double sx, double sy):Entity(w, &Rocket::sprite, x, y) {
		this->sx = sx;
		this->sy = sy;
		stopOnCollide = true;
		hasGravity = false;
		friction = 0.0;
	}
	
	bool update(float dt);
	
	float timer = 0.0;
};