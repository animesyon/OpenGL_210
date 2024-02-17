/*
   Class Name:

      CKitchen

   Description:

      draw kitchen
*/
#include "extensions.h"

#pragma once

class CKitchen
{
// variable

private:
	    GLuint id1;
	    GLsizei stride;
	    GLint coord_per_vertex, coord_per_texture, attrib_per_vertex;
	    GLint vertex_offset, texture_offset;
		GLsizei vertex_count;
	    GLhandleARB program;

		int vertex_size;

// function

private:

public:
	    CKitchen();
	    ~CKitchen();

	    void Create(float z, int width, int height);
	    void Destroy();
	    void Draw(float *matrix);

	    void SetShader(GLhandleARB handle);
};
