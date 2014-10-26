//
//  Chunk_compress.cpp
//  PixelEngine
//
//  Created by Quinn on 10/24/14.
//
//

#include "Chunk.h"

#include <stdlib.h>

#include "Encoder.h"
#include "Decoder.h"

using namespace dgrid;

// helper functions //

static void compressLayer(Chunk::Layer &tiles, util::Encoder &encoder) {
	// structs
	struct div{unsigned pos;tileID tile;};
	struct entry{char type;unsigned pos;int delta;tileID tile;};
	enum EntryType {OFFSET=0, ADD=1, REMOVE=2, END_OF_LINE=3};
	
	// initialize counts
	uint32_t tileCounts[10];  // counts of each tile type encoded
	for (int i=0; i<10; i++) tileCounts[i] = 0;
	uint32_t entryCounts[3]; // counts of each entry type
	for (int i=0; i<3; i++) entryCounts[i] = 0;
	
	// find dividers
	std::vector<div> lines[CHUNK_SIZE];
	std::vector<entry> entries[CHUNK_SIZE];
	for (unsigned x = 0; x < CHUNK_SIZE; x++) {
		// find dividers in this line
		div lastDiv = {0, tiles[x][0]};
		lines[x].push_back(lastDiv);
		tileCounts[lastDiv.tile]++;
		for (unsigned y = 1; y < CHUNK_SIZE; y++) {
			tileID t = tiles[x][y];
			if (t != lastDiv.tile) {
				lines[x].push_back(lastDiv = {y, t});
				tileCounts[t]++;
			}
		}
		
		// convert to output entries
		if (x == 0) {
			// first line; encode all divs as ADD
			for (div d : lines[x]) {
				entries[x].push_back({ADD, d.pos, 0, d.tile});
			}
		} else {
			std::vector<div> &T = lines[x-1], &B = lines[x];
			
			// find OFFSET pairs
			int *bPairD = new int[B.size()]; // distances of matches
			int *bPairI = new int[B.size()]; // indices in T of matches
			for (int i=0; i<B.size(); i++) bPairI[i] = -1;
			for (int ti = 0; ti < T.size(); ti++) {
				// find closest divider in B
				div &t = T[ti];
				int closestI;
				int dist = CHUNK_SIZE+1;
				for (int bi = 0; bi < B.size(); bi++) {
					div &b = B[bi];
					if (b.tile != t.tile) continue;
					int d = abs(t.pos-b.pos);
					if (d < dist) {
						dist = d;
						closestI = bi;
					}
				}
				
				// check if t is new closest to b
				if (dist < bPairD[closestI]) {
					bPairD[closestI] = dist;
					bPairI[closestI] = ti;
				}
			}
			
			
			
			delete [] bPairI;
			delete [] bPairD;
		}
	}
	
	// populate SymbolSets
	util::SymbolSet tileSymbols;
	for (int i = 0; i < 10; i++) {
		tileSymbols.addSymbol(tileCounts[i]);
		if (tileCounts[i] > 0) {
			encoder.encode(i, 256);
			encoder.encode(tileCounts[i], CHUNK_SIZE*CHUNK_SIZE+1);
		}
	}
	util::SymbolSet entrySymbols;
	entrySymbols.addSymbol(entryCounts[OFFSET]);
	entrySymbols.addSymbol(entryCounts[ADD]);
	entrySymbols.addSymbol(entryCounts[REMOVE]);
	entrySymbols.addSymbol(CHUNK_SIZE); // end-of-line symbol
	
	// encode lines
	for (unsigned x = 0; x < CHUNK_SIZE; x++) {
		// encode entries
		unsigned lastPos = 0;
		for (entry e : entries[x]) {
			if (x != 0)
				encoder.encode(entrySymbols, e.type);
			if (e.type == OFFSET) {
				//...
			} else if (e.type == ADD) {
				encoder.encode(e.pos-lastPos, CHUNK_SIZE-lastPos);
				lastPos = e.pos;
				encoder.encode(tileSymbols, e.tile);
			} else if (e.type == REMOVE) {
				// no extra data
			}
		}
		encoder.encode(entrySymbols, END_OF_LINE);
	}
	
	
	
	
	return;
	
	// */ /*
	
	
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
	// */
	
	
}

//////////////////////

#include <fstream>

void Chunk::compress(unsigned char* &data, unsigned int &len) {
	printf("Compressing chunk (%i, %i)...\n", cx, cy);
	util::Encoder encoder;
	
	// encode data
	for (int l = 0; l < NUM_LAYERS-1; l++) {
		compressLayer(layers[l], encoder);
	}
	encoder.finish();
	data = encoder.data();
	len = encoder.len();
	
	unsigned long before = (NUM_LAYERS-1)*CHUNK_SIZE*CHUNK_SIZE*sizeof(tileID), after = encoder.len();
	printf("Bytes before compression: %lu (%.1fkb)\n", before, before/1024.0f);
	printf("Bytes after compression: %lu (%.1fkb)\n", after, after/1024.0f);
	printf("Compression ratio: %f%%\n\n\n", 100*float(after)/float(before));
	
	/*// print bits
	for (unsigned i = 0; i < len; i++) {
		printf("%i", !!(data[i]&1));
		printf("%i", !!(data[i]&2));
		printf("%i", !!(data[i]&4));
		printf("%i", !!(data[i]&8));
		printf("%i", !!(data[i]&16));
		printf("%i", !!(data[i]&32));
		printf("%i", !!(data[i]&64));
		printf("%i", !!(data[i]&128));
	}*/
	std::ofstream file("/Users/quinn/test_compressed.dat");
	file.write((char*)data, len);
	file.close();
	file = std::ofstream("/Users/quinn/test.dat");
	for (int l = 0; l < NUM_LAYERS-1; l++) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
			file.write((char*)layers[l][x], CHUNK_SIZE*sizeof(tileID));
		}
	}
	file.close();
	
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