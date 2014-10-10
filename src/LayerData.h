//
//  LayerData.h
//  PixelEngine
//
//  Created by Quinn on 10/8/14.
//
//

#pragma once

#include "worldFormat.h"

namespace engine {
	
	class LayerData {
	public:
		virtual ~LayerData() {}
		virtual bool setTile(tileID tiles[CHUNK_SIZE][CHUNK_SIZE], int x, int y, tileID newTile) = 0;
	};
	
	class EntityLayerData: public LayerData {
	public:
		EntityLayerData();
		
		virtual bool setTile(tileID tiles[CHUNK_SIZE][CHUNK_SIZE], int x, int y, tileID newTile);
		
		unsigned char counts[CHUNK_SIZE][CHUNK_SIZE];
	};
	
}