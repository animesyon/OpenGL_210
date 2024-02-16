/*
   Class Name:

      CModel


   Description:

      draw circle
*/

#include "extensions.h"

#ifndef CMODEL_H
#define CMODEL_H

class CModel
{

// variable

private:
	    GLuint id;
	    GLsizei vertex_count, stride;
	    GLint coord_per_vertex;
	    GLint vertex_offset;
	    GLhandleARB program;

public:

// function

private:
	    void CreateVertexData(double radius, int subdivision, float **vertex, int *size, int *count);

public:
	    CModel();
	    ~CModel();

	    void Create(double radius, int subdivision);
	    void Destroy();
	    void Render(float *matrix);

	    void PutHandle(GLhandleARB handle);
};

#endif

//
