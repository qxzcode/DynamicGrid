//
//  PixelWorldApp.cpp
//  PixelWorld
//
//  Created by Quinn on 9/11/14.
//
//

#include "PixelWorldApp.h"

#include "input.h"
#include "TileTypes.h"

PixelWorldApp::PixelWorldApp():generator(time(NULL)),world(&generator) {
	
}

void PixelWorldApp::prepareSettings(Settings* settings) {
	settings->setWindowSize(1000, 800);
}

void PixelWorldApp::setup() {
	getWindow()->setTitle("Pixel World");
	world.init();
	initTiles(world.tileTypes);
	getDelta();
}

void PixelWorldApp::shutdown() {
	
}


void PixelWorldApp::resize() {
	
}

void PixelWorldApp::keyDown(KeyEvent event) {
	key(event.getCode(), true);
}

void PixelWorldApp::keyUp(KeyEvent event) {
	key(event.getCode(), false);
}

static bool compress = false;
#include "Chunk.h"
dgrid::Chunk* compChunk;

void PixelWorldApp::key(int code, bool down) {
	switch (code) {
		case KeyEvent::KEY_a:
			leftDown = down;
			break;
		case KeyEvent::KEY_d:
			rightDown = down;
			break;
		case KeyEvent::KEY_SPACE:
			jumpDown = down;
			break;
			
		case KeyEvent::KEY_c:
			compress = down;
			break;
	}
}


void PixelWorldApp::update() {
	// update FPS counter
	float dt = getDelta();
	timer += dt;
	frameCount++;
	if (timer > 1.0) {
		char str[50];
		sprintf(str, "Pixel World (%i FPS)", frameCount);
		getWindow()->setTitle(str);
		timer -= 1.0;
		frameCount = 0;
	}
	
	// update game
	static const float DT_MAX = 1.0f/30.0f; // cap dt to prevent glitches in cases of extreme lag (such as when the world first loads)
	world.update(dt>DT_MAX? DT_MAX : dt);
	
	// TESTING
	if (compress) {
		compress = false;
		printf("Compressing chunk (%i, %i)...\n", compChunk->cx, compChunk->cy);
		dgrid::util::Encoder encoder;
		compChunk->compressLayerChainCode(1, encoder);
		encoder.finish();
		
		unsigned long before = (NUM_LAYERS-1)*CHUNK_SIZE*CHUNK_SIZE*sizeof(tileID), after = encoder.len();
		printf("Bytes before compression: %lu (%.1fkb)\n", before, before/1024.0f);
		printf("Bytes after compression: %lu (%.1fkb)\n", after, after/1024.0f);
		printf("Compression ratio: %f%%\n\n\n", 100*float(after)/float(before));
	}
}

void PixelWorldApp::draw() {
	world.draw(getWindowWidth(), getWindowHeight());
}




CINDER_APP_NATIVE( PixelWorldApp, RendererGl(RendererGl::AA_NONE) )
