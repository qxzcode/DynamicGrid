//
//  util.h
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#pragma once

namespace engine { namespace util {
	int floor(double x) {
		return x>0? int(x) : int(x)-1;
	}
	int ceil(double x) {
		return x>0? int(x)+1 : int(x);
	}
	int floor(float x) {
		return x>0? int(x) : int(x)-1;
	}
	int ceil(float x) {
		return x>0? int(x)+1 : int(x);
	}
	template <typename T>
	T clampMax(T x, T max) {
		return x>max? max : x;
	}
} }