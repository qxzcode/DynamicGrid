//
//  util.h
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#pragma once

namespace dgrid { namespace util {
	inline int floor(double x) {
		return x>0? int(x) : int(x)-1;
	}
	inline int ceil(double x) {
		return x>0? int(x)+1 : int(x);
	}
	inline int floor(float x) {
		return x>0? int(x) : int(x)-1;
	}
	inline int ceil(float x) {
		return x>0? int(x)+1 : int(x);
	}
	template <typename T>
	inline T clampMax(T x, T max) {
		return x>max? max : x;
	}
} }