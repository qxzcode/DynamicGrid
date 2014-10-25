//
//  Chunk_compress.cpp
//  PixelEngine
//
//  Created by Quinn on 10/24/14.
//
//

#include "Chunk.h"

#include "Encoder.h"
#include "Decoder.h"

using namespace dgrid;

// helper functions //

static void compressLayer(Chunk::Layer &tiles, util::Encoder &encoder) {
	struct run{unsigned len,max;tileID tile;};
	
	// parse runs & count symbols
	std::vector<run> runs;
	uint32_t counts[10];
	for (int i=0; i<10; i++) counts[i] = 0;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		run curRun;
		for (int y = 0; y < CHUNK_SIZE; y++) {
			tileID t = tiles[x][y];
			if (y==0) {
				curRun = {0,CHUNK_SIZE,t};
				counts[t]++;
			} else if (t != curRun.tile) {
				runs.push_back(curRun);
				curRun = {0,curRun.max-curRun.len,t};
				counts[t]++;
			} else {
				curRun.len++;
			}
		}
		runs.push_back(curRun);
	}
	
	// populate SymbolSet
	util::SymbolSet symbols;
	for (int i = 0; i < 10; i++) {
		symbols.addSymbol(counts[i]);
		if (counts[i] > 0) {
			encoder.encode(i, 256);
			encoder.encode(counts[i], CHUNK_SIZE*CHUNK_SIZE+1);
			printf("Count of %i: %i\n", i, counts[i]);
		}
	}
	
	// encode runs
	for (run r : runs) {
		encoder.encode(r.len, r.max);
		encoder.encode(symbols, r.tile);
	}
}

//////////////////////

void Chunk::compress(unsigned char* &data, unsigned int &len) {
	printf("Compressing chunk (%i, %i)...\n", cx, cy);
	util::Encoder encoder;
	
	// encode data
	for (int l = 0; l < NUM_LAYERS-1; l++) {
		compressLayer(layers[l], encoder);
	}
	encoder.finish();
	
	unsigned long before = (NUM_LAYERS-1)*CHUNK_SIZE*CHUNK_SIZE*sizeof(tileID), after = encoder.len();
	printf("Bytes before compression: %lu (%.1fkb)\n", before, before/1024.0f);
	printf("Bytes after compression: %lu (%.1fkb)\n", after, after/1024.0f);
	printf("Compression ratio: %f%%\n\n\n", 100*float(after)/float(before));
	
	return;
	
	// check data
	util::Decoder decoder(encoder.data(), encoder.len());
	for (int l = 0; l < NUM_LAYERS-1; l++) {
		util::SymbolSet symbols;
		//		printf("Counts:\n");
		for (int i=0; i<10; i++) {
			uint32_t count = decoder.read(CHUNK_SIZE*CHUNK_SIZE+1);
			symbols.addSymbol(count);
			uint32_t i2 = decoder.read(256);
			if (i2 != i)
				printf("ERROR i=%i, i2=%i\n", i, i2);
			if (count > 0) {
				//				printf("\t%i: %i\n", i, count);
			}
		}
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				if (decoder.read(symbols) != getTile(l, x, y))
					printf("ERROR (%i, %i)\n", x, y);
			}
		}
	}
}