//
//  PixelWorldGen.cpp
//  PixelEngine
//
//  Created by Quinn on 10/9/14.
//
//

#include "PixelWorldGen.h"

#include <cmath>
#include <cstdlib>

#include "TileTypes.h"
#include "LayerData.h"

PixelWorldGen::PixelWorldGen(unsigned long seed):
						baseTerrain(seed++, 5, 0.35),
						grassFray(seed++),
						dirtRocks(seed++) {}

void PixelWorldGen::generateChunk(engine::World* world, engine::Chunk* chunk) {
	// init layer data
//	chunk->layers[2].data = new engine::EntityLayerData();
	
	// fill with simplex noise!
	
	// generate noise
	static const double scale = 128.0;
#define P 8
	bool noiseGrid[CHUNK_SIZE+P*2][CHUNK_SIZE+P*2];
	float noiseVals[CHUNK_SIZE][CHUNK_SIZE];
	for (int x = -P; x < CHUNK_SIZE+P; x++) {
		for (int y = -P; y < CHUNK_SIZE+P; y++) {
			double noise = baseTerrain.getNoise((chunk->cwx+x)/scale, (chunk->cwy+y)/scale);
			noise = fabs(noise)*2.0 - 1.0; // ridged multifractal transform
			if (x>=0&&x<CHUNK_SIZE&&y>=0&&y<CHUNK_SIZE)
				noiseVals[x][y] = noise;
			noiseGrid[x+P][y+P] = noise>-0.5;
		}
	}
	
	// fill tiles
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			// background
			if (noiseVals[x][y]<-0.4) {
				chunk->layers[0][x][y] = SKY;
			} else {
				chunk->layers[0][x][y] = DIRTBG;
			}
			
			// foreground
			if (!noiseGrid[x+P][y+P]) {
				chunk->layers[1][x][y] = EMPTY;
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
				if (grassFray.get(chunk->cwx+x, chunk->cwy+y)%(P/2) > emptyDist-(P/2)) {
					chunk->layers[1][x][y] = GRASS;
				} else {
					chunk->layers[1][x][y] = dirtRocks.get(chunk->cwx+x, chunk->cwy+y)%35? DIRT : STONE;
				}
			}
			
			// entities
			chunk->layers[2][x][y] = EMPTY;
		}
	}
}




