//
//  TileType.cpp
//  PixelEngine
//
//  Created by Quinn on 10/5/14.
//
//

#include "TileType.h"

using namespace dgrid;

TileType::TileType(bool s, Color c1):solid(s),color1(c1),colorChance(0) {}

TileType::TileType(bool s, Color c1, Color c2, int cc):solid(s),color1(c1),color2(c2),colorChance(cc) {}
