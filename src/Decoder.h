//
//  Decoder.h
//  PixelEngine
//
//  Created by Quinn on 10/16/14.
//
//

#pragma once

#include <stdint.h>

#include "SymbolSet.h"

namespace dgrid { namespace util {
	
	typedef unsigned char byte;
	
	class Decoder {
	public:
		Decoder(const byte* data, unsigned long len);
		
		unsigned read(SymbolSet& set);
		uint32_t read(uint32_t max);
		
	protected:
		uint32_t decode(uint32_t total);
		void update(uint32_t low_count, uint32_t high_count);
		bool readBit();
		
		const byte* data;
		unsigned long len, curI;
		uint32_t low, high, step, buffer;
		byte curByte, curBit;
		
	};
	
} }