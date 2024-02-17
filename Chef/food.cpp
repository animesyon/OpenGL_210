/*
   Class Name:

      CFood

   Description:

      draw food
*/

#include "stdafx.h"
#include "food.h"

// constructor
CFood::CFood()
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
CFood::~CFood()
{
}

//
void CFood::GenerateVertices(float *buffer, int *k, float x, float y, float z, float px1, float py1, float px2, float py2, int width, int height)
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
void CFood::Create(float z, float px, float py1, float py2, float py3, float py4, float py5, float x1, float x2, float y1, float y2, float y3, float y4, float y5, float y6, int width, int height)
{
    float* vertex  = NULL;
	int i;

	image_count = 5;
	vertex_size = image_count * vertex_count * attrib_per_vertex;
	vertex   = new float[vertex_size];

	i = 0;

	//      X1    X2 
	//   Y1 +-----+
	//      |     |
	//   Y2 +-----+
	//      |     | 
	//   Y3 +-----+
	//      |     |
	//   Y4 +-----+
	//      |     | 
	//   Y5 +-----+
	//      |     | 
	//   Y6 +-----+
	//
	GenerateVertices(vertex, &i,  px, py1, z, x1, y1, x2, y2, width, height);
	GenerateVertices(vertex, &i,  px, py2, z, x1, y2, x2, y3, width, height);
	GenerateVertices(vertex, &i,  px, py3, z, x1, y3, x2, y4, width, height);
	GenerateVertices(vertex, &i,  px, py4, z, x1, y4, x2, y5, width, height);
	GenerateVertices(vertex, &i,  px, py5, z, x1, y5, x2, y6, width, height);


	// for vertex  vertex
    glGenBuffersARB(1, &id1);                                                                // generate a vertex object
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, id1);                                               // bind the vertex object
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertex_size * sizeof(float), vertex, GL_STATIC_DRAW_ARB);  // copy vertex  data to the vertex object

    // release memory
    if(vertex != NULL) delete[] vertex;
}

// delete buffer object
void CFood::Destroy()
{
    glDeleteBuffersARB(1, &id1);
}

// draw a square composed by two triangles
void CFood::Draw(float *matrix)
{
	if (pos > 4) return;

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
void CFood::SetShader(GLhandleARB handle)
{
	    program = handle;
}

// put food on pan
//
// value of variable pos
//      0
//      1
//      2
//      3
//      4   <--- pan on downward position
//      5
void CFood::PutOnPan()
{
	pos = 4;
	dir = false;

	offset = pos * vertex_count;
}

// move the food by position p
void CFood::Move(int p)
{
	pos = p;
	offset = pos * vertex_count;
}

// move the food in a direction set by dir
// true  - downward
// false - upward
void CFood::Move()
{
	if (dir)
		++pos;
	else
		--pos;

	offset = pos * vertex_count;
}


// any number from 0 to 2
//
//      0   <---
//      1   <---
//      2   <---
//      3
//      4
//      5
void CFood::SetMaxHeight(int h)
{
	maxheight = h;
}

// return true if maxheight is reached
bool CFood::ReachMaxHeight()
{
	return (pos == maxheight);
}

//
void CFood::SetToFallDown()
{
	dir = true;
}

// toggle direction
void CFood::Toggle()
{
	dir = !dir;
}

// value of variable pos
//      0
//      1
//      2
//      3
//      4   <--- food can be flipped at this height
//      5
bool CFood::CanBeFlipped()
{
	return (pos == 4);
}

// value of variable pos
//      0
//      1
//      2   <--- food can be poked at this height
//      3
//      4
//      5
bool CFood::CanBePoked()
{
	return (pos == 2);
}

// value of variable pos
//      0
//      1
//      2
//      3
//      4
//      5   <--- where the mouse steal the food
bool CFood::FallOnFloor()
{
	return (pos == 5);
}
