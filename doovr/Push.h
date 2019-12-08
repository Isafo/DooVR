#pragma once
#include "Tool.h"
class Push : public Tool
{
public:
	Push(DynamicMesh* mesh, Wand* wand);
	~Push();

	void render(MatrixStack* MVstack, GLint locationMV);

	void renderIntersection(MatrixStack* MVstack, GLint locationMV);

	void findIntersection(DynamicMesh* mesh, Wand* wand, int triIndex){};
	void firstSelect(DynamicMesh* mesh, Wand* wand);
	void moveVertices(DynamicMesh* mesh, Wand* wand, float dT);
	void deSelect();

	void setRadius(float rad) { radius = rad; };
	void setStrength(float str){};

	void getIntersection(float* intersectionP, float* intersectionN){};

private:
	float radius;

	vertex* mVertexArray;
	vInfo* mVInfoArray;
	halfEdge* mEdgeArray;
	float* mPosition;
	float* mOrientation;

	float mMAX_LENGTH;

	float tempVec[3];
	float zVec[3];
	float iTransform[16];
};

