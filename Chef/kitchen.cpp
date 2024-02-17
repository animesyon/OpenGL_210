/*
   Class Name:

      CKitchen

   Description:

      draw kitchen
*/

#include "stdafx.h"
#include "kitchen.h"

// constructor
CKitchen::CKitchen()
{
    id1               = 0;
    coord_per_vertex  = 3;                                     // x, y, z
	coord_per_texture = 2;                                     // s, t
	attrib_per_vertex = coord_per_vertex + coord_per_texture;
    stride            = attrib_per_vertex * sizeof(float);     // in bytes
	vertex_offset     = 0;                                     // x y z s t
	texture_offset    = coord_per_vertex * sizeof(float);      // 0 1 2 3 4

	vertex_count      = 4;
}

// destructor
CKitchen::~CKitchen()
{
}

// create vertex buffer object
void CKitchen::Create(float z, int width, int height)
{
    float* buffer  = NULL;
	int i;
	float s1, t1, s2, t2;
	float w, h;
	float x1, y1, x2, y2;
	float X1, Y1, X2, Y2;

	// The texture coordinate of a kitchen are the following:
	X1 = 4.0f;
	X2 = 259.0f;
	Y1 = 6.0f;
	Y2 = 177.0f;

	s1 = X1 / (float)width;
	s2 = X2 / (float)width;
	t1 = Y2 / (float)height;
	t2 = Y1 / (float)height;

	w = X2 - X1 + 1.0f;
	h = Y2 - Y1 + 1.0f;

	x1 = 0.0f;
	y1 = 0.0f;
	x2 = x1 + w;
	y2 = y1 + h;


    //        0            1
    //     y2 +------------+
    //        |            |
    //        |            |
    //        |            |
    //        |            |
    //     y1 +------------+
	//        x1           x2
    //        3            2     

	vertex_size = vertex_count * attrib_per_vertex;
	buffer   = new float[vertex_size];

	i = 0;

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

	// for vertex buffer
    glGenBuffersARB(1, &id1);                                                                // generate a buffer object
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id1);                                               // bind the buffer object
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_size * sizeof(float), buffer, GL_STATIC_DRAW_ARB);  // copy vertex data to the buffer object

    // release memory
    if(buffer != NULL) delete[] buffer;
}

// delete buffer object
void CKitchen::Destroy()
{
    glDeleteBuffersARB(1, &id1);
}

//
void CKitchen::Draw(float *matrix)
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
		glDrawArrays(GL_QUADS, 0, vertex_count);
		glDisableVertexAttribArrayARB(LOC_TEXTURE);
	    glDisableVertexAttribArrayARB(LOC_VERTEX);

		// unbind buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

// set shader to be used
void CKitchen::SetShader(GLhandleARB handle)
{
	    program = handle;
}

//
