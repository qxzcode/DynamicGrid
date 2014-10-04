//
//  OpenGL.h
//  PixelEngine
//
//  Created by Quinn on 10/3/14.
//
//

#pragma once

#include "platform.h"

#if   defined( PLATFORM_MSW )
	#include <windows.h>
	#include <gl/gl.h>
#elif defined( PLATFORM_COCOA_TOUCH )
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#elif defined( PLATFORM_MAC )
	#include <OpenGL/gl.h>
	#include <OpenGL/glext.h>
#endif