/*
   Class Name:

      CHPlane

   Description:

      create horizontal plane
*/

#pragma once



class CHPlane
{
private:
	int vertex_count, index_count;
	VERTEX_DATA_TYPE* vertex;
	INDEX_DATA_TYPE* index;

public:
	CHPlane();
	~CHPlane();

	void Create(float x1, float x2, float z1, float z2);
	void Draw();
};
