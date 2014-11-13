//
//  TileSet.h
//  PixelEngine
//
//  Created by Quinn on 11/12/14.
//
//

#pragma once

#include <vector>

#include "TileType.h"

namespace dgrid {
	
	class TileSet {
	public:
		TileSet& addType(TileType tt);
		TileType& operator[](unsigned t) {
			return types[t];
		}
		
	protected:
		std::vector<TileType> types;
		
	};
	
}