//
//  PixelWorldGen.h
//  PixelEngine
//
//  Created by Quinn on 10/9/14.
//
//

#pragma once

#include "WorldGenerator.h"
#include "SimplexNoise.h"

class PixelWorldGen: public engine::WorldGenerator {
public:
	PixelWorldGen(unsigned long seed);
	
	virtual void generateChunk(engine::World* world, engine::Chunk* chunk);
	
protected:
	// noise sources
	engine::util::FractalNoise baseTerrain;
	engine::util::RandomGrid grassFray, dirtRocks;
	
};