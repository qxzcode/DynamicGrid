//
//  LayerData.h
//  PixelEngine
//
//  Created by Quinn on 10/8/14.
//
//

#pragma once

#include "Chunk.h"

namespace engine {
	
	class LayerData {
	public:
		virtual bool setTile(tileID** tiles, int x, int y, tileID newTile) = 0;
	};
	
	class EntityLayerData {
	public:
		EntityLayerData();
		
		virtual bool setTile(tileID** tiles, int x, int y, tileID newTile);
		
		unsigned char counts[CHUNK_SIZE][CHUNK_SIZE];
	};
	
}