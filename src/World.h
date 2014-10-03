//
//  World.h
//  PixelWorld
//
//  Created by Quinn on 9/11/14.
//
//

#pragma once

#include <map>
#include <vector>
#include <forward_list>

#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/GlslProg.h"
using namespace ci;

#include "worldFormat.h"
#include "chunkCoords.h"
#include "ChunkLoader.h"
#include "SimplexNoise.h"
#include "Entity.h"
#include "Player.h"
#include "util.h"


class Chunk;

class World {
public:
	World();
	World(const World& w) = delete;
	World& operator=(const World& rhs) = delete;
	~World();
	
	void init();
	void update(float dt);
	void draw(int winWidth, int winHeight);
	
	tileID getTile(int x, int y);
	void setTile(int x, int y, tileID tile);
	
	void spawnParticle(int x, int y);
	void spawnEntity(Entity* e);
	
	int camX() const {return util::floor(player->x);}
	int camY() const {return util::floor(player->y);}
	
	Player* player;
	
	// noise for generation and rendering
protected:
	struct NoiseContainer {
		NoiseContainer(unsigned long seed):baseTerrain(seed++, 5, 0.35),grassFray(seed++),dirtRocks(seed++),tileColors(seed++) {}
		
		// generation
		FractalNoise baseTerrain;
		RandomGrid grassFray, dirtRocks;
		
		// rendering
		RandomGrid tileColors;
	};
public: NoiseContainer noise;
	
	
	
protected:
	static int getChunkCoord(int c) {
		int c2 = c<0? c - (CHUNK_SIZE-1) : c;
		return c2/CHUNK_SIZE;
	}
	static int getLocalCoord(int c) {
		int mod = c%CHUNK_SIZE;
		if (c >= 0 || mod==0) return mod;
		else return CHUNK_SIZE + mod;
	}
	Chunk& loadChunk(int cx, int cy);
	bool chunkLoaded(int cx, int cy) {
		std::lock_guard<std::mutex> lock(chunkLoader.chunksMutex);
		return chunks.find({cx, cy}) != chunks.end();
	}
	
	int cxMin = 0, cyMin = 0, cxMax = -1, cyMax = -1;
	std::forward_list<Chunk*> visibleChunks;
	
	std::map<chunkCoords, Chunk*> chunks;
	void addChunk(Chunk* chunk);
	
	class ChunkLoader chunkLoader;
	friend class ChunkLoader;
	
	struct EntityRef {
		Entity* ptr;
		EntityRef(Entity* ptr):ptr(ptr) {}
		~EntityRef() { delete ptr; }
		Entity* operator->() { return ptr; }
	};
	std::forward_list<EntityRef> entities;
	
	gl::GlslProg gameShader;
};






