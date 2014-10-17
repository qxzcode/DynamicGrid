//
//  SymbolSet.h
//  PixelEngine
//
//  Created by Quinn on 10/16/14.
//
//

#pragma once

#include <stdint.h>
#include <vector>

namespace dgrid { namespace util {
	
	class SymbolSet {
	public:
		SymbolSet();
		
		void addSymbol(uint32_t p);
		
		friend class Encoder;
		friend class Decoder;
		
	protected:
		std::vector<uint32_t> high_counts, low_counts;
		uint32_t total;
		
	};
	
} }