//
//  ChunkLoader.cpp
//  PixelWorld
//
//  Created by Quinn on 9/20/14.
//
//

#include "ChunkLoader.h"

void ChunkLoader::worker() {
	while (true) {
		// get the next request from the queue
		chunkCoords cc;
		{
			std::unique_lock<std::mutex> lock(requestsMutex);
			bool stopped;
			while (!(stopped=isStopped()) && requests.empty()) {
				hasRequest.wait(lock);
			}
			if (stopped) break;
			cc = requests.front();
			requests.pop_front();
		}
		
		// load the chunk and add it to the world's loaded chunk list
		Chunk chunk(cc.x, cc.y);
		chunk.generate(world);
		world->addChunk(chunk);
		
		// notify threads waiting for a chunk to load
		chunkLoaded.notify_all();
	}
}


void ChunkLoader::startWorker() {
	workerThread = std::thread(&ChunkLoader::worker, this);
}

void ChunkLoader::stopWorker() {
	stopMutex.lock();
	stop = true;
	stopMutex.unlock();
	hasRequest.notify_all();
	workerThread.join();
}


void ChunkLoader::requestLoadChunk(chunkCoords cc) {
	if (world->chunkLoaded(cc.x, cc.y)) return;
	std::lock_guard<std::mutex> lock(requestsMutex);
	for (chunkCoords& cc2 : requests)
		if (cc == cc2) return;
	requests.push_front(cc);
	hasRequest.notify_all();
}

void ChunkLoader::loadChunk(chunkCoords cc) {
	requestLoadChunk(cc);
	std::unique_lock<std::mutex> lock(requestsMutex);
	while (!world->chunkLoaded(cc.x, cc.y)) {
		chunkLoaded.wait(lock);
	}
}



