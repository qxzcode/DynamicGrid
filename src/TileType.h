//
//  TileType.h
//  PixelEngine
//
//  Created by Quinn on 10/5/14.
//
//

#pragma once

#include "Color.h"

namespace dgrid {
	
	class TileType {
	public:
		TileType(bool s, Color c1);
		TileType(bool s, Color c1, Color c2, int cc);
		
		Color getColor(int rand) {
			return rand<colorChance? color2 : color1;
		}
		
		bool solid;
		Color color1, color2;
		int colorChance;
	};
	
}
