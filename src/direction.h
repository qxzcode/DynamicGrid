//
//  direction.h
//  PixelEngine
//
//  Created by Quinn on 11/15/14.
//
//

#pragma once

namespace dir {
	typedef char dir;
	
	enum {
		FORWARD = 0, XP = 0,
		BACKWARD = 2, XN = 2,
		RIGHT = 1, YP = 1,
		LEFT = 3, YN = 3,
	};
	
	dir rotateC(dir d);
	dir rotateCC(dir d);
	dir comp(dir d1, dir d2);
	
	void moveDir(double &x, double &y, dir d, double amt);
	void moveDir(int &x, int &y, dir d, int amt);
}
