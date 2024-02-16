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
    m.a[0] = 0.0f;    m.a[4] = 0.0f;    m.a[8]  = 0.0f;    m.a[12] = 0.0f;
    m.a[1] = 0.0f;    m.a[5] = 0.0f;    m.a[9]  = 0.0f;    m.a[13] = 0.0f;
    m.a[2] = 0.0f;    m.a[6] = 0.0f;    m.a[10] = 0.0f;    m.a[14] = 0.0f;
    m.a[3] = 0.0f;    m.a[7] = 0.0f;    m.a[11] = 0.0f;    m.a[15] = 0.0f;
}

// constructor
CMatrix4::CMatrix4(float* s)
{
	int i;

	for(i=0;i<MATRIX4_SIZE;i++)
	{
		m.a[i] = s[i];
	}
}

// destructor
CMatrix4::~CMatrix4()
{
}

// Orthographic Projection Matrix
void CMatrix4::Orthographic(float l, float r, float b, float t, float n, float f)
{
    m.a[0] = 2.0f/(r - l);     m.a[4] = 0.0f;             m.a[8]  = 0.0f;             m.a[12] = -(r + l)/(r - l);
    m.a[1] = 0.0f;             m.a[5] = 2.0f/(t - b);     m.a[9]  = 0.0f;             m.a[13] = -(t + b)/(t - b);
    m.a[2] = 0.0f;             m.a[6] = 0.0f;             m.a[10] = -2.0f/(f - n);    m.a[14] = -(f + n)/(f - n);
    m.a[3] = 0.0f;             m.a[7] = 0.0f;             m.a[11] = 0.0f;             m.a[15] = 1.0f;
}

// Scaling Matrix
void CMatrix4::Scale(float x, float y, float z)
{
    m.a[0] = x;       m.a[4] = 0.0f;    m.a[8]  = 0.0f;    m.a[12] = 0.0f;
    m.a[1] = 0.0f;    m.a[5] = y;       m.a[9]  = 0.0f;    m.a[13] = 0.0f;
    m.a[2] = 0.0f;    m.a[6] = 0.0f;    m.a[10] = z;       m.a[14] = 0.0f;
    m.a[3] = 0.0f;    m.a[7] = 0.0f;    m.a[11] = 0.0f;    m.a[15] = 1.0f;
}

// Overloaded = sign
void CMatrix4::operator=(CMatrix4 s)
{
	int i;

	for(i=0;i<MATRIX4_SIZE;i++)
	{
		m.a[i] = s.m.a[i];
	}
}

// Overloaded * sign
CMatrix4 CMatrix4::operator*(CMatrix4 s)
{
	int i, j, k;
	union array1d2d sum;

	//   OpenGL matrix (column-major)           matrix laid out in memory (row major)
    //   | 0  4  8 12 |   | 0  4  8 12 |        |  0   1   2   3 |   |  0   1   2   3 |
    //   | 1  5  9 13 |   | 1  5  9 13 |        |  4   5   6   7 |   |  4   5   6   7 |
    //   | 2  6 10 12 |   | 2  6 10 12 |        |  8   9  10  11 |   |  8   9  10  11 |
    //   | 3  7 11 13 |   | 3  7 11 13 |        |  12 13  14  15 |   | 12  13  14  15 |

	for(i=0;i<MATRIX4_DIM;i++)
	{
		for(j=0;j<MATRIX4_DIM;j++)
		{
			sum.b[j][i] = 0.0f;

			for(k=0;k<MATRIX4_DIM;k++)
			{
				sum.b[j][i] += (m.b[k][i] * s.m.b[j][k]);
			}
		}
	}

	return CMatrix4(sum.a);
}

// operator to cast to a float*
CMatrix4::operator float* ()
{
    return m.a;
}

//
