//
//  TileTypes.h
//  PixelWorld
//
//  Created by Quinn on 9/14/14.
//
//

#pragma once

#define EMPTY 0
#define EMPTY_COLOR {152, 231, 255}

#define DIRT 1
#define DIRT_COLOR1 {172, 101, 49}
#define DIRT_COLOR2 {185, 108, 53}
#define DIRT_COLOR_RATE 17
#define DIRT_SOLID true

#define GRASS 2
#define GRASS_COLOR1 {0, 198, 31}
#define GRASS_COLOR2 {0, 187, 29}
#define GRASS_COLOR_RATE 30
#define GRASS_SOLID true

#define STONE 3
#define STONE_COLOR1 {173, 173, 173}
#define STONE_COLOR2 {163, 163, 163}
#define STONE_COLOR_RATE 30
#define STONE_SOLID true


/// the color(s) for each tile type
static const unsigned char TILE_COLORS[][2][3] = {
	{EMPTY_COLOR},
	{DIRT_COLOR1,DIRT_COLOR2},
	{GRASS_COLOR1,GRASS_COLOR2},
	{STONE_COLOR1,STONE_COLOR2},
};
/// the chance (out of 100) to pick the secondary color
static const int TILE_COLOR_RATES[] = {
	0,
	DIRT_COLOR_RATE,
	GRASS_COLOR_RATE,
	STONE_COLOR_RATE,
};
static const bool TILE_SOLID[] = {
	false,
	DIRT_SOLID,
	GRASS_SOLID,
	STONE_SOLID,
};