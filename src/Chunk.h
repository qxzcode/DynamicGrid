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

namespace dgrid {
	
	class Chunk {
	public:
		Chunk(int cx, int cy);
		~Chunk();
		
		void draw(class World *world);
		
		tileID getTile(int l, int x, int y) { return layers[l][x][y]; }
		void setTile(int l, int x, int y, tileID tile);
		
		// COMPRESSION STUFF //
		void compress(unsigned char* &data, unsigned &len);
		///////////////////////
		
		friend class World;
		
//	protected:
		const int cx, cy, cwx, cwy;
		struct Layer {
			~Layer();
			tileID* operator[](int x) {
				return tiles[x];
			}
			tileID tiles[CHUNK_SIZE][CHUNK_SIZE];
			class LayerData* data = NULL;
		};
		Layer layers[NUM_LAYERS];
		
	protected:
		void init();
		bool initialized = false;
		
		bool dirty = true;
		int dirtyXMin = 0, dirtyXMax = CHUNK_SIZE-1, dirtyYMin = 0, dirtyYMax = CHUNK_SIZE-1;
		
		GLuint tileTex = 0;
		GLuint displayList = 0;
	};
	
}