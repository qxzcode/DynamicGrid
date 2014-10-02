//
//  Chunk.h
//  PixelWorld
//
//  Created by Quinn on 9/11/14.
//
//

#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/DisplayList.h"
#include "cinder/gl/Texture.h"
using namespace ci;

typedef unsigned char tileID;
#define CHUNK_SIZE 256	// width/height of a chunk in tiles

class Chunk {
public:
	Chunk(int cx, int cy);
	~Chunk();
	
	void generate(class World *world);
	void draw(World *world);
	
	tileID getTile(int x, int y) { return tiles[x][y]; }
	void setTile(int x, int y, tileID tile);
	
	friend class World;
	
protected:
	const int cx, cy, cwx, cwy;
	tileID tiles[CHUNK_SIZE][CHUNK_SIZE];
	
	void init();
	bool initialized = false;
	
	bool dirty = true;
	int dirtyXMin = 0, dirtyXMax = CHUNK_SIZE-1, dirtyYMin = 0, dirtyYMax = CHUNK_SIZE-1;
	
	gl::Texture tileTex;
	GLuint displayList = 0;
};