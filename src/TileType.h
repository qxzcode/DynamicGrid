//
//  TileType.h
//  PixelEngine
//
//  Created by Quinn on 10/5/14.
//
//

#pragma once

#include "Color.h"

namespace engine {
	
	class TileType {
	public:
		TileType(bool s, Color c1);
		TileType(bool s, Color c1, Color c2, int cc);
		
		bool solid;
		Color color1, color2;
		int colorChance;
	};
	
}
