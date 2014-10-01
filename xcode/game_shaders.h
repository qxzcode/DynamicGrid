//
//  game_shaders.h
//  PixelWorld
//
//  Created by Quinn on 9/27/14.
//
//

#pragma once

static const char* game_vert_src = "\
varying vec4 color;\
\
void main() {\
	gl_TexCoord[0] = gl_MultiTexCoord0;\
	color = gl_Color;\
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\
}\
";

static const char* game_frag_src = "\
uniform sampler2D texture;\
\
varying vec4 color;\
\
void main() {\
	gl_FragColor = texture2D(texture, gl_TexCoord[0].st) * color;\
}\
";