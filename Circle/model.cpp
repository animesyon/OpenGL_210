/*
   Class Name:

      CModel


   Description:

      draw circle
*/


#include "stdafx.h"
#include "model.h"

// constructor
CModel::CModel()
{
    id               = 0;
    coord_per_vertex = 3;                                   // 3 coordinate per vertex (x, y, z)
    stride           = coord_per_vertex * sizeof(float);    // in bytes

    vertex_offset = 0;
}

// destructor
CModel::~CModel()
{
}

// create vertex data
void CModel::CreateVertexData(double radius, int subdivision, float **vertex, int *size, int *count)
{

	*count = subdivision;                     // number of subdivisions
	*size  = subdivision * coord_per_vertex;  // size of an array

	*vertex = new float[*size];               // allocate a one-dimensional array
	                                          // then fill it with vertices
	float x, y, z, a, rad, d;
	int i, k;

	d = 360.0f / (float)subdivision;        // degree per subdivision

	a = 0.0f;
	k = 0;
	for(i=0;i<subdivision;++i)
	{
		rad = (a/180.0f) * (float)M_PI;     // convert to radian

		y = (float)(radius * sin(rad));
		x = (float)(radius * cos(rad));
		z = 0.0f;

		(*vertex)[k++] = x;                // vertex
		(*vertex)[k++] = y;
		(*vertex)[k++] = z;

		a += d;
	}
}

// create vertex buffer object
void CModel::Create(double radius, int subdivision)
{
    float* vertex   = NULL;
    int vertex_size = 0;

    // create vertex data
    CreateVertexData(radius, subdivision, &vertex, &vertex_size, &vertex_count);

    glGenBuffersARB(1, &id);                                                                       // generate a buffer object
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);                                                      // bind the buffer object
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_size * sizeof(float), vertex, GL_STATIC_DRAW_ARB); // copy vertex data to the buffer object 

    // release memory
    if(vertex != NULL) delete[] vertex;
}

// delete buffer object
void CModel::Destroy()
{
    glDeleteBuffersARB(1, &id);
}

// render
void CModel::Render(float *matrix)
{
	    GLint loc_m;

		// bind the buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);

		// send matrix to a vertex shader
	    loc_m = glGetUniformLocationARB(program, "m_matrix");
	    glUniformMatrix4fvARB(loc_m, 1, false, matrix);

		// send vertices to a vertex shader
	    glVertexAttribPointerARB(LOC_VERTEX, coord_per_vertex, GL_FLOAT, GL_FALSE, stride, (GLvoid*)vertex_offset);

		// draw circle using the line loop
	    glEnableVertexAttribArrayARB(LOC_VERTEX);
	    glDrawArrays(GL_LINE_LOOP, 0, vertex_count);
	    glDisableVertexAttribArrayARB(LOC_VERTEX);

		// unbind buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

// put a shader program handle to be used
// this is a handle returned by the class function CShader::GetHandle()
void CModel::PutHandle(GLhandleARB handle)
{
	    program = handle;
}

//
