//
//  chunkCoords.h
//  PixelWorld
//
//  Created by Quinn on 9/20/14.
//
//

#pragma once

namespace engine {
	
	struct chunkCoords {
		int x, y;
		bool operator<(const chunkCoords& rhs) const {
			return x==rhs.x? y<rhs.y : x<rhs.x;
		}
		chunkCoords& operator=(const chunkCoords& rhs) {
			x = rhs.x;
			y = rhs.y;
			return *this;
		}
		bool operator==(const chunkCoords& rhs) {
			return x==rhs.x && y==rhs.y;
		}
	};
	
}