//
//  Color.cpp
//  PixelEngine
//
//  Created by Quinn on 10/5/14.
//
//

#include "Color.h"

#include "util.h"

using namespace dgrid;

static inline ch_t mix(unsigned short a, unsigned short b, unsigned short x) {
	return (a*(255-x) + b*x)/255;
}

Color Color::blend(Color bg, Color fg) {
	if (fg.a==255) return fg;
	if (fg.a==0)   return bg;
	return Color(mix(bg.r, fg.r, fg.a), mix(bg.g, fg.g, fg.a), mix(bg.b, fg.b, fg.a), (ch_t)util::clampMax(int(bg.a+fg.a), 255));
}