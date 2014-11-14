//
//  Entity.h
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#pragma once

#include "worldFormat.h"

namespace dgrid {
	
	class Entity {
	public:
		struct Sprite;
		Entity(class World* w, Sprite* sprite, float x, float y, float sx=0, float sy=0):world(w),curSprite(sprite),x(x),y(y),lastX(x),lastY(y),sx(sx),sy(sy) {}
		virtual ~Entity() {}
		
		virtual bool update(float dt);
		void draw();
		void drawSprite(int x, int y);
		void clearSprite(int x, int y);
		
		virtual void onRemove();
		
		double x, y, sx, sy;
		
		struct Sprite {
			Sprite(int w, int h);
			Sprite(const Sprite& s);
			~Sprite();
			
			bool& solid(int x, int y) {
				return solidData[x+y*w];
			}
			tileID& tile(int x, int y) {
				return tileData[x+y*w];
			}
			
			const int w, h;
			bool* const solidData;
			tileID* const tileData;
		};
		
	protected:
		void doCollide();
		bool doCollideStep();
		bool isColliding(double x, double y, int* cx = NULL, int* cy = NULL);
		bool onGround = false, collided = false;
		bool stopOnCollide = false, hasGravity = true;
		
		double lastX, lastY;
		int lastDrawX, lastDrawY;
		bool drawn = false;
		Sprite* curSprite;
		
		World* world;
		
	};
	
}