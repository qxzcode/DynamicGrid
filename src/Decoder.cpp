//
//  Decoder.cpp
//  PixelEngine
//
//  Created by Quinn on 10/16/14.
//
//

#include "Decoder.h"

#include "arithmetic_globals.h"

using namespace dgrid::util;

Decoder::Decoder(const byte* data, unsigned long len):data(data),len(len),curI(0),curByte(data[0]),curBit(0),low(0),high(0x7FFFFFFF) {
	// buffer the first 31 bits
	buffer = 0;
	for (int n = 0; n < 31; n++)
		buffer = buffer*2 + readBit();
}

unsigned Decoder::read(SymbolSet& set) {
	uint32_t v = decode(set.total);
	unsigned num = set.low_counts.size()-1;
	unsigned i;
	for (i = 0; i < num; i++) {
		if (v < set.high_counts[i])
			break;
	}
	update(set.low_counts[i], set.high_counts[i]);
	return i;
}

uint32_t Decoder::read(uint32_t max) {
	uint32_t v = decode(max);
	update(v, v+1);
	return v;
}

uint32_t Decoder::decode(uint32_t total) {
	step = (high - low + 1) / total;
	return (buffer - low) / step;
}

void Decoder::update(uint32_t low_count, uint32_t high_count) {
	// update current interval
	high = low + high_count*step - 1;
	low  = low + low_count*step;
	
	// if needed, buffer bits and apply scaling
	bool e1;
	while ((e1 = high<HALF) || (low>=HALF)) {
		if (e1) {	// E1 scaling (interval in lower half)
			low  = low*2;
			high = high*2 + 1;
			buffer = buffer*2 + readBit();
		} else {	// E2 scaling (interval in upper half)
			low  = (low-HALF)*2;
			high = (high-HALF)*2 + 1;
			buffer = (buffer-HALF)*2 + readBit();
		}
//		if (eof) return;
	}
	while ((QUART1<=low) && (high<QUART3)) {
		low  = (low-QUART1)*2;
		high = (high-QUART1)*2 + 1;
		buffer = (buffer-QUART1)*2 + readBit();
//		if (eof) return;
	}
}

bool Decoder::readBit() {
	bool bit = curByte>>7;
	curByte <<= 1;
	if (++curBit == 8) { // load the next byte, or 0 in case EOF
		curByte = (eof = ++curI>=len)? 0 : data[curI];
		if (eof) printf("EOF\n");
		curBit = 0;
	}
	return bit;
}