
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _USE_MATH_DEFINES               // for M_PI

//
#define LOC_VERTEX  1
#define LOC_TEXTURE 2

// Windows Header Files:
#include <windows.h>
#include <mmsystem.h>

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <gl/gl.h>               // standard OpenGL functions
#include <gl/glu.h>              // OpenGL utility library
#include <gl/glext.h>            // OpenGL extension
								 // glext.h can be downloaded from khronos website
// png headers
#include <png.h>

/*
LINK THESE TO PROJECT

winmm.lib
opengl32.lib
glu32.lib
libpngd.lib
libpng.lib
zlibstatd.lib
zlibstat.lib
*/
