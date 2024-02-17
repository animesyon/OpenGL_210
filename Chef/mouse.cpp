/*
   Class Name:

      CMouse

   Description:

      draw mouse
*/

#include "stdafx.h"
#include "mouse.h"


// constructor
CMouse::CMouse()
{
    id1               = 0;
    coord_per_vertex  = 3;                                     // x, y, z
	coord_per_texture = 2;                                     // s, t
	attrib_per_vertex = coord_per_vertex + coord_per_texture;
    stride            = attrib_per_vertex * sizeof(float);     // in bytes
	vertex_offset     = 0;                                     // x y z s t
	texture_offset    = coord_per_vertex * sizeof(float);      // 0 1 2 3 4

	vertex_count      = 4;

	pos    = 0;
	offset = 0;
}

// destructor
CMouse::~CMouse()
{
}

//
void CMouse::GenerateVertices(float *buffer, int *k, float x, float y, float z, float px1, float py1, float px2, float py2, int width, int height)
{
	int i;
	float x1, y1, x2, y2;
	float s1, t1, s2, t2;
	float cx, cy;

	s1 = px1 / width;
	s2 = px2 / width;

	t1 = py2 / height;
	t2 = py1 / height;

	cx = px2 - px1 + 1.0f;
	cy = py2 - py1 + 1.0f;

	x1 = x;
	y1 = y;
	x2 = x1 + cx;
	y2 = y1 + cy;

    //        0            1
    //     y2 +------------+
    //        |            |
    //        |            |
    //        |            |
    //        |            |
    //        |            |
    //        |            |
    //     y1 +------------+
	//        x1           x2
    //        3            2
	//       
	i = *k;

    // vertex 0
   buffer[i++] = x1;
   buffer[i++] = y2;
   buffer[i++] = z;
   buffer[i++] = s1;
   buffer[i++] = t2;

    // vertex 1
   buffer[i++] = x2;
   buffer[i++] = y2;
   buffer[i++] = z;
   buffer[i++] = s2;
   buffer[i++] = t2;

    // vertex 2
   buffer[i++] = x2;
   buffer[i++] = y1;
   buffer[i++] = z;
   buffer[i++] = s2;
   buffer[i++] = t1;

    // vertex 3
   buffer[i++] = x1;
   buffer[i++] = y1;
   buffer[i++] = z;
   buffer[i++] = s1;
   buffer[i++] = t1;

   *k = i;
}

// create vertex buffer object
void CMouse::Create(float z, int width, int height)
{
    float* vertex  = NULL;
	int i;
	float X1, X2, X3, X4, X5, X6, X7;
	float Y1, Y2;

	image_count = 6;
	vertex_size = image_count * vertex_count * attrib_per_vertex;
	vertex   = new float[vertex_size];

	X1 =   0.0f;
	X2 =  37.0f;
	X3 =  89.0f;
	X4 = 142.0f;
	X5 = 198.0f;
	X6 = 253.0f;
	X7 = 291.0f;

	Y1 = 218.0f;
	Y2 = 255.0f;

	i = 0;

	//      X1    X2    X3    X4    X5    X6    X7
	//   Y1 +-----+-----+-----+-----+-----+-----+
	//      |     |     |     |     |     |     |
	//   Y2 +-----+-----+-----+-----+-----+-----+
	//
	GenerateVertices(vertex, &i,   0, -2, z, X1, Y1, X2, Y2, width, height);
	GenerateVertices(vertex, &i,  29, -2, z, X2, Y1, X3, Y2, width, height);
	GenerateVertices(vertex, &i,  74, -2, z, X3, Y1, X4, Y2, width, height);
	GenerateVertices(vertex, &i, 124, -2, z, X4, Y1, X5, Y2, width, height);
	GenerateVertices(vertex, &i, 174, -2, z, X5, Y1, X6, Y2, width, height);
	GenerateVertices(vertex, &i, 218, -2, z, X6, Y1, X7, Y2, width, height);

	// for vertex  vertex
    glGenBuffersARB(1, &id1);                                                                // generate a vertex object
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id1);                                               // bind the vertex object
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_size * sizeof(float), vertex, GL_STATIC_DRAW_ARB);  // copy vertex  data to the vertex object

    // release memory
    if(vertex != NULL) delete[] vertex;
}

// delete buffer object
void CMouse::Destroy()
{
    glDeleteBuffersARB(1, &id1);
}

// draw a square composed by two triangles
void CMouse::Draw(float *matrix)
{
	GLint loc_m;

		// bind the buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id1);

		// send projection matrix to a vertex shader
	    loc_m = glGetUniformLocationARB(program, "m_matrix");
	    glUniformMatrix4fvARB(loc_m, 1, false, matrix);

		// send vertices to a vertex shader
	    glVertexAttribPointerARB(LOC_VERTEX, coord_per_vertex, GL_FLOAT, GL_FALSE, stride, (GLvoid*)vertex_offset);

		// send texture to a vertex shader
	    glVertexAttribPointerARB(LOC_TEXTURE, coord_per_texture, GL_FLOAT, GL_FALSE, stride, (GLvoid*)texture_offset);

		// draw texture
	    glEnableVertexAttribArrayARB(LOC_VERTEX);
		glEnableVertexAttribArrayARB(LOC_TEXTURE);
		glDrawArrays(GL_QUADS, offset, vertex_count);
		glDisableVertexAttribArrayARB(LOC_TEXTURE);
	    glDisableVertexAttribArrayARB(LOC_VERTEX);

		// unbind buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

// set shader to be used
void CMouse::SetShader(GLhandleARB handle)
{
	    program = handle;
}

//
void CMouse::Move(int i)
{
	pos = i;
	offset = pos * vertex_count;
}

//
void CMouse::Move()
{
	++pos;
	offset = pos * vertex_count;
}

//
int CMouse::GetMoveCount()
{
	return image_count;
}
