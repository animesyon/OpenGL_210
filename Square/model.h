/*
   Class Name:

      CModel

   Description:

      draw image on the screen
*/

#include "extensions.h"

#ifndef CMODEL_H
#define CMODEL_H

class CModel
{

// variable

private:
	    GLuint id1, id2;
	    GLsizei stride;
	    GLint coord_per_vertex;
	    GLint vertex_offset, index_offset;
		GLsizei vertex_size, index_size;
	    GLhandleARB program;


public:

// function

private:
	    void CreateVertexData(float width, float height, float **vertex, int *vertex_size, short **index, int *index_size);

public:
	    CModel();
	    ~CModel();

	    void Create(float width, float height);
	    void Destroy();
	    void Draw(float *matrix);

	    void PutHandle(GLhandleARB handle);
};

#endif

//
