/*
   Class Name:

      CCat

   Description:

      draw cat
*/
#include "extensions.h"

#pragma once

class CCat
{
private:
	GLuint id1;
	GLsizei stride;
	GLint coord_per_vertex, coord_per_texture, attrib_per_vertex;
	GLint vertex_offset, texture_offset;
	GLsizei vertex_count;
	GLhandleARB program;

	int image_count;
	int offset;
	int vertex_size;

private:
	void GenerateVertices(float *buffer, int *k, float x, float y, float z, float px1, float py1, float px2, float py2, int width, int height);

public:
	CCat();
	~CCat();

	void Create(float z, int width, int height);
	void Destroy();
	void Draw(float *matrix);

	void SetShader(GLhandleARB handle);

	void Poke();
	void Idle();

	bool CanPokeHam();

};
