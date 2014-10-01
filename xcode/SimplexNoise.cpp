//
//  SimplexNoise.cpp
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#include "SimplexNoise.h"

#include <stdlib.h>

#include "util.h"

SimplexNoise::SimplexNoise(unsigned long seed):randGrid(seed) {
	
}

double SimplexNoise::getNoise(double xin, double yin) {
	// Skew the input space to determine which simplex cell we're in
	static const double F2 = 0.5*(sqrt(3.0)-1.0);
	double s = (xin+yin)*F2; // Hairy factor for 2D
	int i = util::floor(xin+s);
	int j = util::floor(yin+s);
	
	static const double G2 = (3.0-sqrt(3.0))/6.0;
	double t = (i+j)*G2;
	double X0 = i-t; // Unskew the cell origin back to (x,y) space
	double Y0 = j-t;
	double x0 = xin-X0; // The x,y distances from the cell origin
	double y0 = yin-Y0;
	
	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
	int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
	if (x0>y0) {i1=1; j1=0;} // lower triangle, XY order: (0,0)->(1,0)->(1,1)
	else	   {i1=0; j1=1;} // upper triangle, YX order: (0,0)->(0,1)->(1,1)
	
	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6
	
	double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
	double y1 = y0 - j1 + G2;
	double x2 = x0 - 1.0 + 2.0*G2; // Offsets for last corner in (x,y) unskewed coords
	double y2 = y0 - 1.0 + 2.0*G2;
	
	// Work out the hashed gradient indices of the three simplex corners
//	int ii = i % PERM_TABLE_LEN;
//	if (i<0 && ii!=0)
//		ii = PERM_TABLE_LEN + ii;
//	int jj = j % PERM_TABLE_LEN;
//	if (j<0 && jj!=0)
//		jj = PERM_TABLE_LEN + jj;
	int gi0 = randGrid.get(i,    j)    % 12;
	int gi1 = randGrid.get(i+i1, j+j1) % 12;
	int gi2 = randGrid.get(i+1,  j+1)  % 12;
	
	// Calculate the contribution from the three corners
	double n0, n1, n2; // the contributions
	
	double t0 = 0.5 - x0*x0-y0*y0;
	if (t0<0.0) n0 = 0.0;
	else {
		t0 *= t0;
		n0 = t0 * t0 * dot(grad[gi0], x0, y0); // (x,y) of grad used for 2D gradient
	}
	
	double t1 = 0.5 - x1*x1-y1*y1;
	if (t1<0.0) n1 = 0.0;
	else {
		t1 *= t1;
		n1 = t1 * t1 * dot(grad[gi1], x1, y1);
	}
	
	double t2 = 0.5 - x2*x2-y2*y2;
	if (t2<0.0) n2 = 0.0;
	else {
		t2 *= t2;
		n2 = t2 * t2 * dot(grad[gi2], x2, y2);
	}
	
	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 70.0 * (n0 + n1 + n2);
}

RandomGrid::RandomGrid(unsigned long seed) {
	int p[PERM_TABLE_LEN];
	srandom(seed);
	for (int i = 0; i < PERM_TABLE_LEN; i++)
		perm[i] = p[i] = random()%PERM_TABLE_LEN;
	for (int i = PERM_TABLE_LEN; i < PERM_TABLE_LEN*2; i++)
		perm[i] = p[i-PERM_TABLE_LEN];
}

FractalNoise::FractalNoise(unsigned long seed, int numOctaves, double per):noise(seed), per(per) {
	if (numOctaves > 1) {
		srandom(seed);
		subOctave = new FractalNoise(random(), numOctaves-1);
	} else subOctave = NULL;
}

const int SimplexNoise::grad[12][3] = {{1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0},
									   {1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1},
									   {0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1}};



