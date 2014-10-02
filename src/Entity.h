//
//  Entity.h
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#pragma once

class Entity {
public:
	struct Sprite;
	Entity(class World* w, Sprite* sprite, float x, float y, float sx=0, float sy=0):world(w),curSprite(sprite),x(x),y(y),lastX(x),lastY(y),sx(sx),sy(sy) {}
	virtual ~Entity() {}
	
	virtual bool update(float dt);
	void draw();
	
	double x, y, sx, sy;
	
	struct Sprite {
		Sprite(int w, int h):w(w),h(h),solidData(new bool[w*h]) {
			for (int i = 0; i < w*h; i++) {
				solidData[i] = true;
			}
		}
		Sprite(const Sprite& s):w(s.w),h(s.h),solidData(new bool[s.w*s.h]) {
			for (int i = 0; i < w*h; i++) {
				solidData[i] = s.solidData[i];
			}
		}
		~Sprite() {
			delete [] solidData;
		}
		
		bool& solid(int x, int y) {
			return solidData[x+y*w];
		}
		
		const int w, h;
		bool* const solidData;
	};
	
protected:
	void doCollide();
	bool doCollideStep();
	bool isColliding(double x, double y, int* cx = NULL, int* cy = NULL);
	bool onGround = false, collided = false;
	bool stopOnCollide = false, hasGravity = true;
	
	double lastX, lastY;
	Sprite* curSprite;
	
	World* world;
	
};