//
//  TileTypes.cpp
//  PixelEngine
//
//  Created by Quinn on 11/12/14.
//
//

#include "TileTypes.h"

#include "Color.h"

using dgrid::TileType;
typedef dgrid::Color C;

void initTiles(dgrid::TileSet& tiles) {
	tiles.addType(dgrid::TileType(false, C(0,0,0, 0))); // EMPTY
	tiles.addType(dgrid::TileType(true, C(172, 101, 49), C(185, 108, 53), 17)); // DIRT
	tiles.addType(dgrid::TileType(false, C(0, 198, 31), C(0, 187, 29), 30)); // GRASS
	tiles.addType(dgrid::TileType(true, C(173, 173, 173), C(163, 163, 163), 30)); // STONE
	tiles.addType(dgrid::TileType(false, C(255, 0, 0))); // ACID
}
