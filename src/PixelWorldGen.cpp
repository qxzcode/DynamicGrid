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

extern dgrid::Chunk* compChunk;

void PixelWorldGen::generateChunk(dgrid::World* world, dgrid::Chunk* chunk) {
	// generate noise
	static const double scale = 128.0;
	
	// fill tiles
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			// background
			chunk->layers[0][x][y] = DIRT;
			
			// foreground
			/*if (!noiseGrid[x+P][y+P]) {
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
			}*/
			double noise = baseTerrain.getNoise((chunk->cwx+x)/scale, (chunk->cwy+y)/scale);
			noise = fabs(noise)*1.5 - 0.25; // ridged multifractal transform
			if (grassFray.get(chunk->cwx+x, chunk->cwy+y)%100 < noise*100) {
				chunk->layers[1][x][y] = GRASS;
			} else {
				chunk->layers[1][x][y] = EMPTY;
			}
			
			// entities
			chunk->layers[2][x][y] = EMPTY;
		}
	}
if(chunk->cx==0&&chunk->cy==0)compChunk=chunk;}




