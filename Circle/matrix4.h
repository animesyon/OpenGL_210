/*
   Class Name:

      CMatrix4


   Description:

      4 x 4 Matrix
*/

#ifndef CMATRIX4_H
#define CMATRIX4_H

#define MATRIX4_SIZE 16
#define MATRIX4_DIM   4

union array1d2d{
	float a[MATRIX4_SIZE];
	float b[MATRIX4_DIM][MATRIX4_DIM];
};

class CMatrix4
{

// variable

private:
    union array1d2d m;

public:

// function

private:

public:
    CMatrix4();
	CMatrix4(float* s);
    ~CMatrix4();

    void Orthographic(float l, float r, float b, float t, float n, float f);

	void Scale(float x, float y, float z);

	void operator=(CMatrix4 s);

	CMatrix4 operator*(CMatrix4 s);

    operator float* ();
};

#endif

//
