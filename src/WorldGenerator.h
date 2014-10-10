//
//  WorldGenerator.h
//  PixelEngine
//
//  Created by Quinn on 10/9/14.
//
//

#pragma once

#include "Chunk.h"
#include "World.h"

namespace engine {
	
	class WorldGenerator {
	public:
		virtual void generateChunk(World* world, Chunk* chunk) = 0;
	};
	
}