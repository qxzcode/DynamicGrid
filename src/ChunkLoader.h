//
//  ChunkLoader.h
//  PixelWorld
//
//  Created by Quinn on 9/20/14.
//
//

#pragma once

#include <thread>
#include <list>

#include "chunkCoords.h"

namespace dgrid {
	
	class ChunkLoader {
	public:
		ChunkLoader(class World* world):world(world) {}
		~ChunkLoader() {stopWorker();}
		
		void startWorker();	// starts the worker thread that processes requests
		void stopWorker();	// terminates the worker thread (calls join)
		void requestLoadChunk(struct chunkCoords cc); // requests that a chunk be loaded asynchronously
		void loadChunk(chunkCoords cc);		  // will block until the chunk has been loaded
		
	protected:
		void worker(); // the worker function that is run in a separate thread
		std::thread workerThread;
		bool stop = false;
		bool isStopped() {
			std::lock_guard<std::mutex> lock(stopMutex);
			return stop;
		}
		
		std::list<chunkCoords> requests;
		std::condition_variable hasRequest, chunkLoaded;
		std::mutex requestsMutex, stopMutex, chunksMutex;
		
		World* world;
		
		friend class World;
	};
	
}


#include "World.h"