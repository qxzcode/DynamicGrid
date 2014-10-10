//
//  worldFormat.h
//  PixelEngine
//
//  Created by Quinn on 10/2/14.
//
//

#pragma once


typedef unsigned char tileID;
#define NUM_LAYERS 3							 // number of layers to the world
#define CHUNK_SIZE 256							 // width/height of a chunk in tiles
#define PIXEL_SIZE 4							 // size of a tile on screen
#define CHUNK_PIXEL_SIZE (PIXEL_SIZE*CHUNK_SIZE) // size of a chunk on screen
