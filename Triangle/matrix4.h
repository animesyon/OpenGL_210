/*
   Class Name:

      CMatrix4

   Description:

      4 x 4 Matrix
*/

#ifndef CMATRIX4_H
#define CMATRIX4_H

#define MATRIX4_SIZE 16

class CMatrix4
{

// variable

private:
    float m[MATRIX4_SIZE];

public:

// function

private:

public:
    CMatrix4();
    ~CMatrix4();

    void Orthographic(float l, float r, float b, float t, float n, float f);

    operator float* ();
};

#endif

//
