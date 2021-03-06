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
					TileType tt = world->tileTypes[t];
					Color color = tt.colorChance==0? tt.color1 : tt.getColor(world->noise.tileColors.get(cwx+x, cwy+y)%100);
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

Chunk::Layer::~Layer() {
	delete data;
}



