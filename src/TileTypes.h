//
//  TileTypes.h
//  PixelWorld
//
//  Created by Quinn on 9/14/14.
//
//

#pragma once

#include "Color.h"

#define EMPTY 0
#define EMPTY_COLOR C(0,0,0, 0)

#define SKY 1
#define SKY_COLOR C(152, 231, 255)

#define DIRT 2
#define DIRT_COLOR1 C(172, 101, 49)
#define DIRT_COLOR2 C(185, 108, 53)
#define DIRT_COLOR_RATE 17
#define DIRT_SOLID true

#define GRASS 3
#define GRASS_COLOR1 C(0, 198, 31)
#define GRASS_COLOR2 C(0, 187, 29)
#define GRASS_COLOR_RATE 30
#define GRASS_SOLID true

#define STONE 4
#define STONE_COLOR1 C(173, 173, 173)
#define STONE_COLOR2 C(163, 163, 163)
#define STONE_COLOR_RATE 30
#define STONE_SOLID true


/// the color(s) for each tile type
static const engine::Color TILE_COLORS[][2] = {
#define C engine::Color
	{EMPTY_COLOR},
	{SKY_COLOR},
	{DIRT_COLOR1,DIRT_COLOR2},
	{GRASS_COLOR1,GRASS_COLOR2},
	{STONE_COLOR1,STONE_COLOR2},
#undef C
};
/// the chance (out of 100) to pick the secondary color
static const int TILE_COLOR_RATES[] = {
	0,
	0,
	DIRT_COLOR_RATE,
	GRASS_COLOR_RATE,
	STONE_COLOR_RATE,
};
static const bool TILE_SOLID[] = {
	false,
	false,
	DIRT_SOLID,
	GRASS_SOLID,
	STONE_SOLID,
};