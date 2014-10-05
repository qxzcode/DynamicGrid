//
//  SimplexNoise.h
//  PixelWorld
//
//  Created by Quinn on 9/13/14.
//
//

#pragma once

#define PERM_TABLE_LEN 600

namespace engine { namespace util {
	
	class RandomGrid {
	public:
		RandomGrid(unsigned long seed);
		
		int get(int x, int y) const {
			int xx = x % PERM_TABLE_LEN;
			if (x<0 && xx!=0)
				xx = PERM_TABLE_LEN + xx;
			int yy = y % PERM_TABLE_LEN;
			if (y<0 && yy!=0)
				yy = PERM_TABLE_LEN + yy;
			return perm[xx + perm[yy]];
		}
		
	protected:
		int perm[PERM_TABLE_LEN*2];
	};
	
	class SimplexNoise {
	public:
		SimplexNoise(unsigned long seed);
		
		double getNoise(double x, double y);
		
		const RandomGrid randGrid;
		
	protected:
		
		static const int grad[12][3];
		
		static double dot(const int g[3], double x, double y) {
			return g[0]*x + g[1]*y;
		}
		
	};
	
	class FractalNoise {
	public:
		FractalNoise(unsigned long seed, int numOctaves, double per = 0.5);
		~FractalNoise() {
			delete subOctave;
		}
		
		double getNoise(double x, double y) {
			if (subOctave) {
				return (noise.getNoise(x, y) + subOctave->getNoise(x*2, y*2)*per) / (1+per);
			} else {
				return noise.getNoise(x, y);
			}
		}
		
	protected:
		SimplexNoise noise;
		double per;
		FractalNoise* subOctave;
		
	};
	
} }

