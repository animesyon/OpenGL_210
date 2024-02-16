/*
   Class Name:

      CModel

   Description:

      draw image on the screen
*/

#include "stdafx.h"
#include "model.h"

// constructor
CModel::CModel()
{
    id1 = id2        = 0;
    coord_per_vertex = 3;                                   // 3 coordinate per vertex (x, y, z)
    stride           = coord_per_vertex * sizeof(float);    // in bytes
}

// destructor
CModel::~CModel()
{
}

// create vertex data
void CModel::CreateVertexData(float width, float height, float **vertex, int *vertex_size, short **index, int *index_size)
{
	 GLsizei vertex_count;

	vertex_count = 4;                                 // 4 vertices of a square
	*vertex_size  = vertex_count * coord_per_vertex;  // size of a vertex array

	*vertex = new float[*vertex_size];                // allocate a one-dimensional array
	                                                  // then fill it with vertices
	int i;
	float x1, y1, x2, y2;

	// square with dimension width and height, center at the origin
	//
	//                   y
	//                   ^
	//      vertex 0     |       vertex 1
    //     (x1,y2,0)     |       (x2,y2,0)
    //        +----------y2----------+
    //        |          |           |
    //        |          |           |
	//        |          |           |
    //  -----------------+-----------------> x
	//        x1         |           x2
    //        |          |           |
    //        |          |           |
    //        +----------y1----------+
    //     vertex 3      |        vertex 2
    //     (x1,y1,0)     |        (x2,y1,0)
	//
	x1 = -width / 2;
	y1 = -height / 2;
	x2 =  width / 2;
	y2 =  height / 2;

	i = vertex_offset = 0;   // vertex data start at offset 0

    // vertex 0
    (*vertex)[i++] = x1;    // x
    (*vertex)[i++] = y2;    // y
    (*vertex)[i++] = 0.0f;  // z

    // vertex 1
    (*vertex)[i++] = x2;   // x
    (*vertex)[i++] = y2;   // y
    (*vertex)[i++] = 0.0f; // z

    // vertex 2
    (*vertex)[i++] = x2;   // x
    (*vertex)[i++] = y1;   // y
    (*vertex)[i++] = 0.0f; // z

    // vertex 3
    (*vertex)[i++] = x1;   // x
    (*vertex)[i++] = y1;   // y
    (*vertex)[i++] = 0.0f; // z

	// The coordinates of the four vertices are in the arrays vertex.
	// The array index tells the order in which the vertices are drawn.
	// To draw a triangle, we gonna link three vertices. The left triangle
	// is made from the zeroth, second and third vertex. The right triangle
	// is made from the zeroth, first and second vertex.
	//
    //  vertex 0             vertex 1
    //        +------------+
    //        |\           |
    //        |  \         |
    //        |    \       |
    //        |      \     |
    //        |        \   |
    //        |          \ |
    //        +------------+
    //  vertex 3             vertex 2
    //
	// Left  Triangle = 0, 2, 3
	// Right Triangle = 0, 1, 2

	*index_size = 6;                   // 6 indices for two triangles
	*index = new short[*index_size];   // allocate a one-dimensional array
	                                   // then fill it with indices

	i = index_offset = 0;   // index data start at offset 0

	// left triangle
	(*index)[i++] = 0;
	(*index)[i++] = 2;
	(*index)[i++] = 3;

	// right triangle
	(*index)[i++] = 0;
	(*index)[i++] = 1;
	(*index)[i++] = 2;
}

// create vertex buffer object
void CModel::Create(float width, float height)
{
    float* vertex  = NULL;
	short* index   = NULL;

    // create data for vertex and index
    CreateVertexData(width, height, &vertex, &vertex_size, &index, &index_size);

	// for vertex buffer
    glGenBuffersARB(1, &id1);                                                                       // generate a buffer object
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id1);                                                      // bind the buffer object
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_size * sizeof(float), vertex, GL_STATIC_DRAW_ARB);  // copy vertex data to the buffer object

	// for index buffer
	glGenBuffersARB(1, &id2);                                                                            // generate a buffer object
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, id2);                                                   // bind the buffer object
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, index_size * sizeof(short), index, GL_STATIC_DRAW_ARB); // copy vertex data to the buffer object

    // release memory
    if(vertex != NULL) delete[] vertex;
	if(index  != NULL) delete[] index;
}

// delete buffer object
void CModel::Destroy()
{
    glDeleteBuffersARB(1, &id1);
	glDeleteBuffersARB(1, &id2);
}

// draw a square composed by two triangles
void CModel::Draw(float *matrix)
{
	    GLint loc_m;

		// bind the buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id1);
	    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, id2);

		// send matrix to a vertex shader
	    loc_m = glGetUniformLocationARB(program, "m_matrix");
	    glUniformMatrix4fvARB(loc_m, 1, false, matrix);

		// send vertices to a vertex shader
	    glVertexAttribPointerARB(LOC_VERTEX, coord_per_vertex, GL_FLOAT, GL_FALSE, stride, (GLvoid*)vertex_offset);

		// draw two triangles
	    glEnableVertexAttribArrayARB(LOC_VERTEX);
		glDrawElements(GL_TRIANGLES, index_size, GL_UNSIGNED_SHORT, (GLvoid*)index_offset);
	    glDisableVertexAttribArrayARB(LOC_VERTEX);

		// unbind buffer object
	    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
}

// put a program handle to be used
// this is a handle returned by the class function CShader::GetHandle()
void CModel::PutHandle(GLhandleARB handle)
{
	    program = handle;
}

//
