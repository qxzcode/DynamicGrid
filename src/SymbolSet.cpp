//
//  SymbolSet.cpp
//  PixelEngine
//
//  Created by Quinn on 10/16/14.
//
//

#include "SymbolSet.h"

using namespace dgrid::util;

SymbolSet::SymbolSet():total(0) {
	
}

void SymbolSet::addSymbol(uint32_t p) {
	low_counts.push_back(total==0? 0 : high_counts[high_counts.size()-1]);
	total += p;
	high_counts.push_back(total);
}