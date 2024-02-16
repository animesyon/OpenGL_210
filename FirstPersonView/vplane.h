/*
   Class Name:

      CHPlane

   Description:

      create vertical plane
*/

#pragma once

class CVPlane
{
private:
	int vertex_count, index_count;
	VERTEX_DATA_TYPE* vertex;
	INDEX_DATA_TYPE* index;
	float nx, ny, nz;

public:
	CVPlane();
	~CVPlane();

	void ComputeNormalVector(float* nx, float* ny, float* nz, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

	void Create(float x1, float z1, float x2, float z2, float h);
	void Draw();
};
