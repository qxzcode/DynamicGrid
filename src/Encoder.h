//
//  Encoder.h
//  PixelEngine
//
//  Created by Quinn on 10/15/14.
//
//

#pragma once

#include <stdint.h>
#include <vector>

#include "SymbolSet.h"

namespace dgrid { namespace util {
	
	typedef unsigned char byte;
	
	class Encoder {
	public:
		Encoder();
		
		void encode(uint32_t low_count, uint32_t high_count, uint32_t total);
		void encode(SymbolSet& set, unsigned sym);
		void encode(SymbolSet& set, unsigned sym, unsigned minSym);
		void encode(uint32_t num, uint32_t max);
		void finish();
		
		byte* data() { return &bytes[0]; }
		std::vector<byte>::size_type len() { return bytes.size(); }
		
	protected:
		void pushBit(bool bit);
		
		uint32_t low, high;
		unsigned scales;
		std::vector<byte> bytes;
		unsigned curI, curBits;
		
	};
	
} }