/*
   Class Name:

      CHPlane

   Description:

      create vertical plane
*/

#include "stdafx.h"
#include "vplane.h"

// constructor
CVPlane::CVPlane()
{
	vertex_count = 0;
	index_count = 0;

	vertex = NULL;
	index = NULL;

	nx = ny = nz = 0.0f;
}

// destructor
CVPlane::~CVPlane()
{
	if(vertex != NULL) delete[] vertex;
	if(index != NULL)  delete[] index;
}

// normal vector point to viewer
// vector a is from vertex 1 to vertex 3
// vector b is from vertex 1 to vertex 2
//          1
//         /\
//        /  \
//     a /    \ b
//      /      \
//     /________\
//    3          2
//
void CVPlane::ComputeNormalVector(float* nx, float* ny, float* nz, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	float ra, rb, rc;
	float ax, ay, az, bx, by, bz, cx, cy, cz;

	// get vector a, from vertex 1 to vertex 3
	ax = x3 - x1;
	ay = y3 - y1;
	az = z3 - z1;

	// normalize vector
	ra = (float)sqrt(ax*ax + ay*ay + az*az);
	ax /= ra;
	ay /= ra;
	az /= ra;

	// get vector b, from vertex 1 to vertex 2
	bx = x2 - x1;
	by = y2 - y1;
	bz = z2 - z1;

	// normalize vector
	rb = (float)sqrt(bx*bx + by*by + bz*bz);
	bx /= rb;
	by /= rb;
	bz /= rb;

	// get cross product of vector a and vector b
	// to get normal vector
	cx = (ay * bz) - (az * by);
	cy = (az * bx) - (ax * bz);
	cz = (ax * by) - (ay * bx);

	// normalize vector
	rc = (float)sqrt(cx*cx + cy*cy + cz*cz);
	*nx = cx / rc;
	*ny = cy / rc;
	*nz = cz / rc;
}

//
void CVPlane::Create(float x1, float z1, float x2, float z2, float h)
{
	//
	//   (x1, h, z1)  +
	//                |\
	//                | \
	//                |  \
	//                |   \
	//   (x1, 0, z1)  +    |
	//                 \   |
	//                  \  |
	//                   \ |
	//                    \|
	//                     +
	//                     (x2, 0, z2)
	//

	ComputeNormalVector(&nx, &ny, &nz, x1, h, z1, x2, 0.0f, z2, x1, 0, z1);

	float r, rx, rz;
	float s, d;
	float x, y, z;
	int i, j, k, m, n;

	rx = x2 - x1;
	rz = z2 - z1;

	r = (float)sqrt(rx*rx + rz*rz);
	rx /= r;
	rz /= r;

	d = 0.4f;
	m = (int)(r / d) + 2;
	n = (int)(h / d) + 1;


	if(vertex != NULL) delete[] vertex;

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
	y = 0;
	while (y < h)
	{

		s = 0.0f;
		x = x1;
		z = z1;
		while (s < r)
		{
			vertex[k].x = x;
			vertex[k].y = y;
			vertex[k].z = z;

			x += (rx*d);
			z += (rz*d);

			++k;
			s += d;

		}

		vertex[k].x = x2;
		vertex[k].y = y;
		vertex[k].z = z2;


		++k;
		y += d;

	}

	if(index != NULL)  delete[] index;

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
void CVPlane::Draw()
{
	int i;

	glBegin(GL_QUADS);
	glNormal3f(nx, ny, nz);
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
