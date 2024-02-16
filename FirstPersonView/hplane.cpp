/*
   Class Name:

      CHPlane

   Description:

      create horizontal plane
*/

#include "stdafx.h"
#include "hplane.h"

// constructor
CHPlane::CHPlane()
{
	vertex_count = 0;
	index_count = 0;

	vertex = NULL;
	index = NULL;
}

// destructor
CHPlane::~CHPlane()
{
	if(vertex != NULL) delete[] vertex;
	if(index != NULL)  delete[] index;
}

//                    
//  x2  +--------------+
//      |              |
//      |              | 
//      |              |
//  x1  +--------------+
//      z1             z2
//
void CHPlane::Create(float x1, float x2, float z1, float z2)
{
	float d, x, z, a, b;
	int i, j, k, m, n;

	a = (float)fabs(x2 - x1);
	b = (float)fabs(z2 - z1);

	d = 0.4f;
	m = (int)(b / d);
	n = (int)(a / d);

	vertex_count  = m * n;
	vertex = new VERTEX_DATA_TYPE[vertex_count];

	// create vertices
	//              0     1     2     3     .     .     .                             m
	//           0  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	//              |     |     |     |     |     |     |     |     |     |     |     |
	//           1  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	//              |     |     |     |     |     |     |     |     |     |     |     |
	//           .  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	//              |     |     |     |     |     |     |     |     |     |     |     |
	//           .  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	//              |     |     |     |     |     |     |     |     |     |     |     |
	//           n  +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
	//

	k = 0;
	x = x1;
	for(i=0;i<n;i++)
	{
		z = z1;
		for (j = 0; j < m; j++)
		{
			vertex[k].x = x;
			vertex[k].y = 0.0f;
			vertex[k].z = z;

			++k;
			z += d;
		}
		x += d;
	}

	index_count = (m - 1)*(n - 1);
	index = new INDEX_DATA_TYPE[index_count];

	// access vertices by index
	//
	//     1      2
	//     j     j+1
	//   i +------+
	//     |      |
	//     |      |
	//     |      |
	// i+1 +------+
	//     4      3

	k = 0;
	for (i = 0; i < (n - 1); ++i)
	{
		for (j = 0; j < (m - 1); ++j)
		{
			index[k].i1 = i*m + j;
			index[k].i2 = i*m + (j+1);
			index[k].i3 = (i+1)*m + (j+1);
			index[k].i4 = (i+1)*m + j;

			++k;
		}
	}
}

//
void CHPlane::Draw()
{
	int i;

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	for(i=0;i<index_count;++i)
	{
         glVertex3f(vertex[index[i].i1].x, vertex[index[i].i1].y, vertex[index[i].i1].z);
         glVertex3f(vertex[index[i].i2].x, vertex[index[i].i2].y, vertex[index[i].i2].z);
         glVertex3f(vertex[index[i].i3].x, vertex[index[i].i3].y, vertex[index[i].i3].z);
         glVertex3f(vertex[index[i].i4].x, vertex[index[i].i4].y, vertex[index[i].i4].z);
	}
	glEnd();
}

//
