//
//  Encoder.cpp
//  PixelEngine
//
//  Created by Quinn on 10/15/14.
//
//

#include "Encoder.h"

using namespace dgrid::util;

Encoder::Encoder():low(0),high(0x7FFFFFFF),scales(0),curI(0),curBits(0) {
	bytes.push_back(0);
}

static const uint32_t QUART1 = 0x20000000;
static const uint32_t HALF   = 0x40000000;
static const uint32_t QUART3 = 0x60000000;

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
			pushBit(0);
			low  = low*2;
			high = high*2 + 1;
			
			for (; scales>0; scales--)
				pushBit(1);
		} else {	// E2 scaling (interval in upper half)
			pushBit(1);
			low  = (low-HALF)*2;
			high = (high-HALF)*2 + 1;
			
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

void Encoder::encode(uint32_t num, uint32_t max) {
	encode(num, num+1, max);
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
