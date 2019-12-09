#include "Mesh.h"


Mesh::Mesh()
	: vertexArray(nullptr)
	, nrofVerts(0)
	, triangleArray(nullptr)
	, nrofTris(0)
	, position{ 0.0f, 0.0f, 0.0f }
	, orientation{ 1.0f, 0.0f, 0.0f, 0.0f,
				   0.0f, 1.0f, 0.0f, 0.0f,
				   0.0f, 0.0f, 1.0f, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f }
{
}


Mesh::~Mesh()
{
}

void Mesh::getOrientation(float* mat){
	for (int i = 0; i < 16; i++) {
		mat[i] = orientation[i];
	}
}