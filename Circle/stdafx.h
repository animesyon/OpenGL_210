// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _USE_MATH_DEFINES               // for M_PI

//
#define LOC_VERTEX 1

// Windows Header Files:
#include <windows.h>

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

/*
link these libs into the project

opengl32.lib
glu32.lib
*/
