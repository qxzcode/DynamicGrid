//
//  direction.cpp
//  PixelEngine
//
//  Created by Quinn on 11/15/14.
//
//

#include "direction.h"

dir::dir dir::rotateC(dir d) {
	return (d+1)%4;
}

dir::dir dir::rotateCC(dir d) {
	return (d-1)%4;
}

dir::dir dir::comp(dir d1, dir d2) {
	return (d1+d2)%4;
}


void dir::moveDir(double &x, double &y, dir d, double amt) {
	switch (d) {
		case 0: x += amt; break;
		case 1: y += amt; break;
		case 2: x -= amt; break;
		case 3: y -= amt; break;
	}
}

void dir::moveDir(int &x, int &y, dir d, int amt) {
	switch (d) {
		case 0: x += amt; break;
		case 1: y += amt; break;
		case 2: x -= amt; break;
		case 3: y -= amt; break;
	}
}