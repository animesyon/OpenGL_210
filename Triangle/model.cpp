/*
   Class Name:

      CModel

   Description:

      draw the triangle
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
void CModel::CreateVertexData(float **vertex, int *size, int *count)
{
	*count = 3;                          // 3 vertices of a triangle
	*size  = *count * coord_per_vertex;  // size of an array

	*vertex = new float[*size];          // allocate a one-dimensional array
	                                     // then fill it with vertices

    // vertex 1
    (*vertex)[0] = 12.73f;   // x
    (*vertex)[1] = 12.73f;   // y
    (*vertex)[2] =  0.0f;    // z

    // vertex 2
    (*vertex)[3] =   4.66f;   // x
    (*vertex)[4] = -17.39f;   // y
    (*vertex)[5] =   0.0f;    // z

    // vertex 3
    (*vertex)[6] = -17.39f;   // x
    (*vertex)[7] =   4.66f;   // y
    (*vertex)[8] =   0.0f;    // z
}

// create vertex buffer object
void CModel::Create()
{
    float* vertex = NULL;
    int vertex_size = 0;

    // create vertex data
    CreateVertexData(&vertex, &vertex_size, &vertex_count);

    glGenBuffersARB(1, &id);                                                                       // generate a buffer object
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);                                                      // bind the buffer object
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_size * sizeof(float), vertex, GL_STATIC_DRAW_ARB); // copy vertex data to the buffer object 

    // set vertex attribute
    glVertexAttribPointerARB(LOC_VERTEX, coord_per_vertex, GL_FLOAT, GL_FALSE, stride, (GLvoid*)vertex_offset);
    glEnableVertexAttribArrayARB(LOC_VERTEX);

    // release memory
    if (vertex != NULL) delete[] vertex;
}

// delete buffer object
void CModel::Destroy()
{
    glDeleteBuffersARB(1, &id);
}

// draw the triangle
void CModel::Render(float *matrix)
{
    GLint loc_m;

    loc_m = glGetUniformLocationARB(program, "m_matrix");
    glUniformMatrix4fvARB(loc_m, 1, false, matrix);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id);
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

// put a program handle to be used
// this is a handle returned by the class function CShader::GetHandle()
void CModel::PutHandle(GLhandleARB handle)
{
	    program = handle;
}

//
