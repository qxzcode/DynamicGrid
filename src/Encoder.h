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

namespace dgrid { namespace util {
	
	typedef unsigned char byte;
	
	class Encoder {
	public:
		Encoder();
		
		void encode(uint32_t low_count, uint32_t high_count, uint32_t total);
		byte* data() { return &bytes[0]; }
		std::vector<byte>::size_type len() { return bytes.size(); }
		
	protected:
		void pushBit(bool bit);
		
		uint32_t low, high;
		int scales;
		std::vector<byte> bytes;
		int curI, curBits;
		
	};
	
} }