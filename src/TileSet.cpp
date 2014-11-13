//
//  TileSet.cpp
//  PixelEngine
//
//  Created by Quinn on 11/12/14.
//
//

#include "TileSet.h"

using namespace dgrid;

TileSet& TileSet::addType(TileType tt) {
	types.push_back(tt);
	return *this;
}