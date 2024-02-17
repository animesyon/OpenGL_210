/*
   Class Name:

      CScore

   Description:

      draw number
*/

#include "extensions.h"
#include "matrix4.h"

#pragma once
class CScore
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
		int pos[4], offset;
		int vertex_size;

		int digit_count;
		float x[4], y, z;

		int value;

public:
	

// function

private:
	void GenerateVertices(float *buffer, int *k, float x, float y, float z, float px1, float py1, float px2, float py2, int width, int height);
	void Refresh();

public:
	    CScore();
	    ~CScore();

	    void Create(float z, int width, int height);
	    void Destroy();
	    void Digit(float *matrix);
		void Draw(float *matrix);

	    void SetShader(GLhandleARB handle);

		void operator=(int num);

	    CScore& operator++();       // Prefix increment operator.  
	    CScore operator++(int);     // Postfix increment operator.  

};
