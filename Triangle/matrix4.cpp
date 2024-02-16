/*
   Class Name:

      CMatrix4

   Description:

      4 x 4 Matrix
*/

#include "stdafx.h"
#include "matrix4.h"

// constructor
CMatrix4::CMatrix4()
{
    m[0] = 0.0f;    m[4] = 0.0f;    m[8]  = 0.0f;    m[12] = 0.0f;
    m[1] = 0.0f;    m[5] = 0.0f;    m[9]  = 0.0f;    m[13] = 0.0f;
    m[2] = 0.0f;    m[6] = 0.0f;    m[10] = 0.0f;    m[14] = 0.0f;
    m[3] = 0.0f;    m[7] = 0.0f;    m[11] = 0.0f;    m[15] = 0.0f;
}

// destructor
CMatrix4::~CMatrix4()
{
}

// Orthographic Projection Matrix
void CMatrix4::Orthographic(float l, float r, float b, float t, float n, float f)
{
    m[0] = 2.0f/(r - l);     m[4] = 0.0f;             m[8]  = 0.0f;             m[12] = -(r + l)/(r - l);
    m[1] = 0.0f;             m[5] = 2.0f/(t - b);     m[9]  = 0.0f;             m[13] = -(t + b)/(t - b);
    m[2] = 0.0f;             m[6] = 0.0f;             m[10] = -2.0f/(f - n);    m[14] = -(f + n)/(f - n);
    m[3] = 0.0f;             m[7] = 0.0f;             m[11] = 0.0f;             m[15] = 1.0f;
}

// operator to cast to a float*
CMatrix4::operator float* ()
{
    return m;
}

//
