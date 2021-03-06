//
//  Encoder.cpp
//  PixelEngine
//
//  Created by Quinn on 10/15/14.
//
//

#include "Encoder.h"

#include "arithmetic_globals.h"

using namespace dgrid::util;

Encoder::Encoder():low(0),high(0x7FFFFFFF),scales(0),curI(0),curBits(0) {
	bytes.push_back(0);
}

void Encoder::encode(uint32_t low_count, uint32_t high_count, uint32_t total) {
	// compute step
	uint32_t step = (high - low + 1) / total;
	
	// update current interval
	high = low + high_count*step - 1;
	low  = low + low_count*step;
	
	// if possible, write bits and apply scaling
	bool e1;
	while ((e1 = high<HALF) || (low>=HALF)) {
		if (e1) {	// E1 scaling (interval in lower half)
			low  = low*2;
			high = high*2 + 1;
			
			pushBit(0);
			for (; scales>0; scales--)
				pushBit(1);
		} else {	// E2 scaling (interval in upper half)
			low  = (low-HALF)*2;
			high = (high-HALF)*2 + 1;
			
			pushBit(1);
			for (; scales>0; scales--)
				pushBit(0);
		}
	}
	while ((QUART1<=low) && (high<QUART3)) {
		scales++;
		low  = (low-QUART1)*2;
		high = (high-QUART1)*2 + 1;
	}
}

void Encoder::encode(SymbolSet& set, unsigned sym) {
	encode(set.low_counts[sym], set.high_counts[sym], set.total);
}

void Encoder::encode(SymbolSet& set, unsigned sym, unsigned minSym) {
	unsigned d = set.low_counts[minSym];
	encode(set.low_counts[sym]-d, set.high_counts[sym]-d, set.total-d);
}

void Encoder::encode(uint32_t num, uint32_t max) {
	encode(num, num+1, max);
}

void Encoder::finish() {
	// properly terminate the stream of bits
	if (low < QUART1) {	// low < QUART1 < HALF <= high
		pushBit(0);
		for (int n=0; n<scales+1; n++) // 1 + [E3 scales]
			pushBit(1);
	} else {			// low < HALF < QUART3 <= high
		pushBit(1);
		// decoder adds zeros automatically
	}
	// shift the last byte into place and remove the tail
	while (curBits != 0)
		pushBit(0);
	bytes.pop_back();
}

void Encoder::pushBit(bool bit) {
	byte& curByte = bytes[curI];
	curByte = (curByte*2) + bit;
	if (++curBits == 8) {
		bytes.push_back(0);
		curI++;
		curBits = 0;
	}
}
