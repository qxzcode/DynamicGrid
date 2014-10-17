//
//  Chunk.cpp
//  PixelWorld
//
//  Created by Quinn on 9/11/14.
//
//

#include "Chunk.h"

#include "World.h"
#include "Color.h"
#include "LayerData.h"
#include "TileTypes.h"
#include "Encoder.h"
#include "Decoder.h"

using namespace dgrid;

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHUNK_SIZE, CHUNK_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
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

void Chunk::draw(World *world) {
	if (!initialized) {
		init();
		initialized = true;
	}
	
	glBindTexture(GL_TEXTURE_2D, tileTex);
	if (dirty) {
		// update the dirty part of the texture
		int width = dirtyXMax-dirtyXMin+1, height = dirtyYMax-dirtyYMin+1;
		unsigned char data[width*height*4];
		int i = 0;
		for (int x = dirtyXMin; x <= dirtyXMax; x++) {
			for (int y = dirtyYMin; y <= dirtyYMax; y++) {
				Color tileColor;
				for (int l = 0; l < NUM_LAYERS; l++) {
					tileID t = layers[l][x][y];
					const Color color = TILE_COLORS
											[t]
											[world->noise.tileColors.get(cwx+x, cwy+y)%100 < TILE_COLOR_RATES[t]];
					tileColor = Color::blend(tileColor, color);
				}
				data[i++] = tileColor.r;
				data[i++] = tileColor.g;
				data[i++] = tileColor.b;
				data[i++] = tileColor.a;
			}
		}
		glTexSubImage2D(GL_TEXTURE_2D, 0, dirtyYMin, dirtyXMin, height, width, GL_RGBA, GL_UNSIGNED_BYTE, data);
		
		dirty = false;
	}
	
	// draw quad
	glCallList(displayList);
}

void Chunk::setTile(int l, int x, int y, tileID tile) {
	bool changed;
	if (layers[l].data) {
		changed = layers[l].data->setTile(layers[l].tiles, x, y, tile);
	} else {
		if (layers[l][x][y] != tile) {
			layers[l][x][y] = tile;
			changed = true;
		} else changed = false;
	}
	
	// if the tile was changed, update the dirty rect
	if (changed) {
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

void Chunk::compress(unsigned char* &data, unsigned int &len) {
	printf("Compressing chunk (%i, %i)...\n", cx, cy);
	util::Encoder encoder;
	
	// encode data
	for (int l = 0; l < NUM_LAYERS-1; l++) {
		uint32_t counts[10];
		for (int i=0; i<10; i++) counts[i] = 0;
		
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				counts[getTile(l, x, y)]++;
			}
		}
		util::SymbolSet symbols;
		printf("Counts for layer %i:\n", l);
		for (int i=0; i<10; i++) {
			symbols.addSymbol(counts[i]);
			
			encoder.encode(counts[i], CHUNK_SIZE*CHUNK_SIZE+1);
			encoder.encode(i, 256);
			if (counts[i] > 0) {
				printf("\t%i: %i (%f%%)\n", i, counts[i], 100*float(counts[i])/float(CHUNK_SIZE*CHUNK_SIZE));
			}
		}
		
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				encoder.encode(symbols, getTile(l, x, y));
			}
		}
	}
	encoder.finish();
	
	unsigned long before = (NUM_LAYERS-1)*CHUNK_SIZE*CHUNK_SIZE*sizeof(tileID), after = encoder.len();
	printf("Bytes before compression: %lu (%lukb)\n", before, before/1024);
	printf("Bytes after compression: %lu (%lukb)\n", after, after/1024);
	printf("Compression ratio: %f%%\n\n\n", 100*float(after)/float(before));
	
	// check data
	util::Decoder decoder(encoder.data(), encoder.len());
	for (int l = 0; l < NUM_LAYERS-1; l++) {
		util::SymbolSet symbols;
//		printf("Counts:\n");
		for (int i=0; i<10; i++) {
			uint32_t count = decoder.read(CHUNK_SIZE*CHUNK_SIZE+1);
			symbols.addSymbol(count);
			uint32_t i2 = decoder.read(256);
			if (i2 != i)
				printf("ERROR i=%i, i2=%i\n", i, i2);
			if (count > 0) {
//				printf("\t%i: %i\n", i, count);
			}
		}
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				if (decoder.read(symbols) != getTile(l, x, y))
					printf("ERROR (%i, %i)\n", x, y);
			}
		}
	}
}

Chunk::Layer::~Layer() {
	delete data;
}



