//
//  platform.h
//  PixelEngine
//
//  Created by Quinn on 10/3/14.
//
//

#pragma once

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	#define PLATFORM_MSW
#elif defined(linux) || defined(__linux) || defined(__linux__)
	#define PLATFORM_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
	#define PLATFORM_COCOA
	#include "TargetConditionals.h"
	#if TARGET_OS_IPHONE
		#define PLATFORM_COCOA_TOUCH
	#else
		#define PLATFORM_MAC
	#endif
#else
	#error "Unknown platform"
#endif