//
//  LayerData.cpp
//  PixelEngine
//
//  Created by Quinn on 10/8/14.
//
//

#include "LayerData.h"

using namespace engine;

EntityLayerData::EntityLayerData() {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			counts[x][y] = 0;
		}
	}
}

inline bool EntityLayerData::setTile(tileID** tiles, int x, int y, tileID newTile) {
	if (newTile!=0 || --counts[x][y]==0) {
		tiles[x][y] = newTile;
		return true;
	} else {
		return false;
	}
}
