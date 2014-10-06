//
//  Chunk.h
//  PixelWorld
//
//  Created by Quinn on 9/11/14.
//
//

#pragma once

#include "OpenGL.h"

#include "worldFormat.h"

namespace engine {
	
	class Chunk {
	public:
		Chunk(int cx, int cy);
		~Chunk();
		
		void generate(class World *world);
		void draw(World *world);
		
		tileID getTile(int l, int x, int y) { return layers[l][x][y]; }
		void setTile(int l, int x, int y, tileID tile);
		
		friend class World;
		
	protected:
		const int cx, cy, cwx, cwy;
		struct Layer {
			tileID tiles[CHUNK_SIZE][CHUNK_SIZE];
			tileID* operator[](int x) {
				return tiles[x];
			}
		};
		Layer layers[NUM_LAYERS];
		
		void init();
		bool initialized = false;
		
		bool dirty = true;
		int dirtyXMin = 0, dirtyXMax = CHUNK_SIZE-1, dirtyYMin = 0, dirtyYMax = CHUNK_SIZE-1;
		
		GLuint tileTex = 0;
		GLuint displayList = 0;
	};
	
}