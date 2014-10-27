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
	uint32_t offsetCounts[CHUNK_SIZE*2]; // counts of each offset value
	for (int i=0; i<CHUNK_SIZE*2; i++) offsetCounts[i] = 0;
	
	// find dividers
	std::vector<div> lines[CHUNK_SIZE];
	std::vector<entry> entries[CHUNK_SIZE];
	for (unsigned x = 0; x < CHUNK_SIZE; x++) {
		// find dividers in this line
		div lastDiv = {0, tiles[x][0]};
		lines[x].push_back(lastDiv);
		for (unsigned y = 1; y < CHUNK_SIZE; y++) {
			tileID t = tiles[x][y];
			if (t != lastDiv.tile) {
				lines[x].push_back(lastDiv = {y, t});
			}
		}
		
		// convert to output entries
		if (x == 0) {
			// first line; encode all divs as ADD
			for (div d : lines[x]) {
				entries[x].push_back({ADD, d.pos, 0/*unused*/, d.tile});
				tileCounts[d.tile]++;
			}
		} else {
			std::vector<div> &T = lines[x-1], &B = lines[x];
			
			// find OFFSET pairs
			int *tPairD = new int[T.size()]; // deltas of matches
			int *tPairI = new int[T.size()]; // indices in B of matches
			for (int i=0; i<T.size(); i++) {tPairI[i] = -1;tPairD[i] = CHUNK_SIZE+1;}
			for (int bi = 0; bi < B.size(); bi++) {
				// find closest divider in T
				div &b = B[bi];
				int closestI = -1;
				int dist = CHUNK_SIZE+1;
				for (int ti = 0; ti < T.size(); ti++) {
					div &t = T[ti];
					if (b.tile != t.tile) continue;
					int d = b.pos-t.pos;
					if (abs(d) < abs(dist)) {
						dist = d;
						closestI = ti;
					}
				}
				
				if (closestI == -1) continue;
				
				// check if b is new closest to t
				if (dist < tPairD[closestI]) {
					tPairD[closestI] = dist;
					tPairI[closestI] = bi;
				}
			}
			
			// find matches in B
			bool *bMatched = new bool[B.size()];
			for (int i=0; i<B.size(); i++) bMatched[i] = false;
			for (int i=0; i<T.size(); i++) if (tPairI[i]!=-1) bMatched[tPairI[i]] = true;
			
			// convert to entries
			for (int ti=0, bi=0; ti<T.size(); ti++) {
				// OFFSET & REMOVE entries
				if (tPairI[ti] != -1) { // if this t has a match in B
					// OFFSET entry
					entries[x].push_back({OFFSET, B[tPairI[ti]].pos, tPairD[ti]});
					entryCounts[OFFSET]++;
					offsetCounts[tPairD[ti]+CHUNK_SIZE]++;
				} else {
					// t isn't matched; REMOVE entry
					entries[x].push_back({REMOVE});
					entryCounts[REMOVE]++;
				}
				// ADD entries
				for (; bi<B.size()&&(ti==T.size()-1||B[bi].pos<=T[ti+1].pos); bi++) {
					if (!bMatched[bi]) {
						entries[x].push_back({ADD, B[bi].pos, 0/*unused*/, B[bi].tile});
						entryCounts[ADD]++;
						tileCounts[B[bi].tile]++;
					}
				}
			}
			
			delete [] tPairI;
			delete [] tPairD;
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
//	util::SymbolSet offsetSymbols;
//	for (int i = 0; i < CHUNK_SIZE*2; i++) {
//		offsetSymbols.addSymbol(offsetCounts[i]);
//		if (offsetCounts[i] > 0) {
////			encoder.encode(i, CHUNK_SIZE*2);
////			encoder.encode(offsetCounts[i], CHUNK_SIZE*CHUNK_SIZE+1);
//			printf("Offset %i: count %i\n", i-CHUNK_SIZE, offsetCounts[i]);
//		}
//	}
	util::SymbolSet entrySymbols;
	entrySymbols.addSymbol(entryCounts[OFFSET]);
	entrySymbols.addSymbol(entryCounts[ADD]);
	entrySymbols.addSymbol(entryCounts[REMOVE]);
	entrySymbols.addSymbol(CHUNK_SIZE); // end-of-line symbol
	
	// encode lines
	for (unsigned x = 0; x < CHUNK_SIZE; x++) {
		// encode entries for this line
		unsigned lastPos = 0;
		for (entry e : entries[x]) {
			if (x != 0) // all entries on first line are ADD
				encoder.encode(entrySymbols, e.type);
			if (e.type == OFFSET) {
				//// TMP ////
				util::SymbolSet offsetSymbols;
				unsigned tPos = e.pos-e.delta;
				int minI = (lastPos-tPos)+CHUNK_SIZE;
				for (int i = minI; i < CHUNK_SIZE*2; i++) {
					offsetSymbols.addSymbol(offsetCounts[i]);
					if (offsetCounts[i] > 0) {
						//			encoder.encode(i, CHUNK_SIZE*2);
						//			encoder.encode(offsetCounts[i], CHUNK_SIZE*CHUNK_SIZE+1);
						//printf("Offset %i: count %i\n", i-CHUNK_SIZE, offsetCounts[i]);
					}
				}
				/////////////
//				encoder.encode(offsetSymbols, e.delta+CHUNK_SIZE-minI);
				lastPos = e.pos;
			} else if (e.type == ADD) {
				encoder.encode(e.pos-lastPos, CHUNK_SIZE-lastPos);
				lastPos = e.pos;
				encoder.encode(tileSymbols, e.tile);
			} else if (e.type == REMOVE) {
				// no extra data
			}
		}
		// encode end-of-line
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