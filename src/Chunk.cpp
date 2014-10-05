//
//  Chunk.cpp
//  PixelWorld
//
//  Created by Quinn on 9/11/14.
//
//

#include "Chunk.h"

#include "World.h"
#include "TileTypes.h"

using namespace engine;

Chunk::Chunk(int cx, int cy):cx(cx),cy(cy),cwx(cx*CHUNK_SIZE),cwy(cy*CHUNK_SIZE) {
	
}

void Chunk::init() {
	// initialize tileTex (will be populated with data in Chunk::draw())
	glGenTextures(1, &tileTex);
	glBindTexture(GL_TEXTURE_2D, tileTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CHUNK_SIZE, CHUNK_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	// initialize mesh and buffer data
	displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
	glBegin(GL_QUADS);
	{
		float x = cx*CHUNK_PIXEL_SIZE, y = cy*CHUNK_PIXEL_SIZE;
		glTexCoord2f(0, 0);
		glVertex2f(x, y);
		glTexCoord2f(0, 1);
		glVertex2f(x+CHUNK_PIXEL_SIZE, y);
		glTexCoord2f(1, 1);
		glVertex2f(x+CHUNK_PIXEL_SIZE, y+CHUNK_PIXEL_SIZE);
		glTexCoord2f(1, 0);
		glVertex2f(x, y+CHUNK_PIXEL_SIZE);
	}
	glEnd();
	glEndList();
}

Chunk::~Chunk() {
	if (displayList)
		glDeleteLists(displayList, 1);
	if (tileTex)
		glDeleteTextures(1, &tileTex);
}

void Chunk::generate(World *world) {
	// fill with simplex noise!
	
	// generate noise
	static const double scale = 128.0;
#define P 8
	bool noiseGrid[CHUNK_SIZE+P*2][CHUNK_SIZE+P*2];
	for (int x = -P; x < CHUNK_SIZE+P; x++) {
		for (int y = -P; y < CHUNK_SIZE+P; y++) {
			double noise = world->noise.baseTerrain.getNoise((cwx+x)/scale, (cwy+y)/scale);
			noise = fabs(noise)*2.0 - 1.0; // ridged multifractal transform
			noiseGrid[x+P][y+P] = noise>-0.5;
		}
	}
	
	// fill tiles
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			if (!noiseGrid[x+P][y+P]) {
				tiles[x][y] = EMPTY;
			} else {
				int emptyDist = P;
				for (int x2 = x; x2 <= x+P*2; x2++) {
					int dx = abs(x2 - (x+P));
					for (int y2 = y; y2 <= y+P*2; y2++) {
						int dy = abs(y2 - (y+P));
						if (!noiseGrid[x2][y2]) {
							int d = dx>dy?dx:dy;
							if (d < emptyDist)
								emptyDist = d;
						}
					}
				}
				if (world->noise.grassFray.get(cwx+x, cwy+y)%(P/2) > emptyDist-(P/2)) {
					tiles[x][y] = GRASS;
				} else {
					tiles[x][y] = world->noise.dirtRocks.get(cwx+x, cwy+y)%35? DIRT : STONE;
				}
			}
		}
	}
}

void Chunk::draw(World *world) {
	if (!initialized) {
		init();
		initialized = true;
	}
	
	glBindTexture(GL_TEXTURE_2D, tileTex);
	if (dirty) {
		// update the dirty part of the texture
		int width = dirtyXMax-dirtyXMin+1, height = dirtyYMax-dirtyYMin+1;
		unsigned char data[width*height*3];
		int i = 0;
		for (int x = dirtyXMin; x <= dirtyXMax; x++) {
			for (int y = dirtyYMin; y <= dirtyYMax; y++) {
				tileID t = tiles[x][y];
				const unsigned char* const color = TILE_COLORS
														[t]
														[world->noise.tileColors.get(cwx+x, cwy+y)%100 <
															TILE_COLOR_RATES[t]];
				data[i++] = color[0];
				data[i++] = color[1];
				data[i++] = color[2];
			}
		}
		glTexSubImage2D(GL_TEXTURE_2D, 0, dirtyYMin, dirtyXMin, height, width, GL_RGB, GL_UNSIGNED_BYTE, data);
		
		dirty = false;
	}
	
	// draw quad
	glCallList(displayList);
}

void Chunk::setTile(int x, int y, tileID tile) {
	if (tiles[x][y] != tile) {
		tiles[x][y] = tile;
		if (!dirty) {
			dirtyXMin=dirtyXMax=x;
			dirtyYMin=dirtyYMax=y;
		} else {
			if (x>dirtyXMax) dirtyXMax=x;
			if (x<dirtyXMin) dirtyXMin=x;
			if (y>dirtyYMax) dirtyYMax=y;
			if (y<dirtyYMin) dirtyYMin=y;
		}
		dirty = true;
	}
}



