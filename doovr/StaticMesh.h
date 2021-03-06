#pragma once
#include "Mesh.h"

struct texCoords {
	GLfloat u;
	GLfloat v;
};

struct sBufferData {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
	GLfloat u;
	GLfloat v;
};

class StaticMesh : public Mesh
{
public:
	StaticMesh();
	~StaticMesh();

	void render();
	void createBuffers();

	void load(std::string fileName);
	void create(float* sourceVerticies, int* sourceIndicies, int vertexCount, int triangleCount);

private:
	texCoords* texArray;
};

