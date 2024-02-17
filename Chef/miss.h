/*
   Class Name:

      CMiss

   Description:

      draw mouse
*/
#include "extensions.h"
#pragma once
class CMiss
{
// variable

private:
	    GLuint id1;
	    GLsizei stride;
	    GLint coord_per_vertex, coord_per_texture, attrib_per_vertex;
	    GLint vertex_offset, texture_offset;
		GLsizei vertex_count;
	    GLhandleARB program;

		int image_count;
		int pos, offset;
		int vertex_size;

		int quad_vertex_count;
		int value;


public:
	

// function

private:
	void GenerateVertices(float *buffer, int *k, float x, float y, float z, float px1, float py1, float px2, float py2, int width, int height);

public:
	    CMiss();
	    ~CMiss();

	    void Create(float z, int width, int height);
	    void Destroy();
	    void Draw(float *matrix);

	    void SetShader(GLhandleARB handle);

		void operator=(int num);

	    CMiss& operator++();       // Prefix increment operator.  
	    CMiss operator++(int);     // Postfix increment operator.  
};
