/*
   Class Name:

      CFood

   Description:

      draw food
*/

#include "extensions.h"

#pragma once
class CFood
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

		bool dir;
		int maxheight;

public:
	

// function

private:
	void GenerateVertices(float *buffer, int *k, float x, float y, float z, float px1, float py1, float px2, float py2, int width, int height);

public:
	CFood();
	 ~CFood();

	void Create(float z, float px, float py1, float py2, float py3, float py4, float py5, float x1, float x2, float y1, float y2, float y3, float y4, float y5, float y6, int width, int height);
	void Destroy();
	void Draw(float *matrix);

	void SetShader(GLhandleARB handle);

	void PutOnPan();
	void Move(int p);
	void Move();
	void SetMaxHeight(int h);
	bool ReachMaxHeight();
	void SetToFallDown();
	void Toggle();
	bool CanBeFlipped();
	bool CanBePoked();
	bool FallOnFloor();
};
