//
//  Encoder.cpp
//  PixelEngine
//
//  Created by Quinn on 10/15/14.
//
//

#include "Encoder.h"

using namespace dgrid::util;

Encoder::Encoder():low(0),high(0x7FFFFFFF),curByte(0),curBits(0) {
	
}

void Encoder::encode(uint32_t low_count, uint32_t high_count, uint32_t total) {
	// compute step
	uint32_t step = (high - low + 1) / total;
	
	// update current interval
	high = low + high_count*step - 1;
	low  = low + low_count*step;
	
	// if possible, write bits and apply scaling
	//...
}

void Encoder::pushBit(bool bit) {
	curByte = (curByte*2) + bit;
	if (++curBits == 8) {
		bytes.push_back(curByte);
		curByte = curBits = 0;
	}
}
