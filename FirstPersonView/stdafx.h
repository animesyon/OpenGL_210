// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _USE_MATH_DEFINES               // for M_PI

typedef struct
{
   float x, y, z;
}VERTEX_DATA_TYPE;

typedef struct
{
	short i1, i2, i3, i4;
}INDEX_DATA_TYPE;

// Windows Header Files:
#include <windows.h>
#include <time.h>  

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <gl/gl.h>               // Standard opengl include.
#include <gl/glu.h>              // Opengl utilities.

/*
link these libs into the project

opengl32.lib
glu32.lib
*/
