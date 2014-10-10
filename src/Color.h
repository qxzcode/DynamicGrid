//
//  Color.h
//  PixelEngine
//
//  Created by Quinn on 10/5/14.
//
//

#pragma once

namespace dgrid {
	
	typedef unsigned char ch_t;
	
	class Color {
	public:
		Color():r(0),g(0),b(0),a(0) {}
		Color(ch_t r, ch_t g, ch_t b, ch_t a = 255):r(r),g(g),b(b),a(a) {}
		
		static Color blend(Color bg, Color fg);
		
		ch_t r, g, b, a;
	};
	
}