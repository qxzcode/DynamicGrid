//
//  TileTypes.h
//  PixelWorld
//
//  Created by Quinn on 9/14/14.
//
//

#pragma once

#include "TileSet.h"

enum {
	EMPTY=0, DIRT, GRASS, STONE, WALL, SCRAP, ACID,
};

void initTiles(dgrid::TileSet& tiles);
