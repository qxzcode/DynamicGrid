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

class PixelWorldGen: public dgrid::WorldGenerator {
public:
	PixelWorldGen(unsigned long seed);
	
	virtual void generateChunk(dgrid::World* world, dgrid::Chunk* chunk);
	
protected:
	// noise sources
	dgrid::util::FractalNoise baseTerrain;
	dgrid::util::RandomGrid grassFray, dirtRocks;
	
};