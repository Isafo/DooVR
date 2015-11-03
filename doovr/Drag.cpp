#include "Drag.h"
#include "LineSphere.h"
#include "math.h"


Drag::Drag(DynamicMesh* mesh, Wand* wand)
{
	selectedVertices = new int[MAX_SELECTED]; selectedSize = 0;


	radius = 0.01f;
	toolBrush = new LineSphere(0.0f, 0.0f, 0.0f, 1.0f);

	mVertexArray = mesh->vertexArray;
	mVInfoArray = mesh->vInfoArray;
	mEdgeArray = mesh->e;
	mPosition = mesh->position;
	mOrientation = mesh->orientation;
	mMAX_LENGTH = mesh->MAX_LENGTH;

	zVec[0] = 0.0f;  zVec[1] = 0.0f; zVec[2] = 1.0f;

	selectedVertices[0] = 101;
}


Drag::~Drag()
{
	delete selectedVertices;
	delete toolBrush;
}


void Drag::render(MatrixStack* MVstack, GLint locationMV){

	MVstack->push();
	MVstack->scale(radius);
	MVstack->translate(toolBrush->getPosition());
	glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack->getCurrentMatrix());
	toolBrush->render();
	MVstack->pop();
}

void Drag::renderIntersection(MatrixStack* MVstack, GLint locationMV) {

}

void Drag::findIntersection(DynamicMesh* mesh, Wand* wand, int triIndex){

	float newWPoint[4];
	float Dirr[4]; float newDirr[4];
	float tempVec1[3]; float tempVec2[3];
	float wPoint[4]; float* vPoint; float* vPoint2;
	int index; int index2;

	int tempEdge;

	bool success = false;

	float pLength = 0.0f;
	float oLength = 0.0f;

	float mLength;
	float minLength;

	linAlg::transpose(mOrientation);
	//--< 1.0 | calculated the position and direction of the wand
	wand->getPosition(wPoint);
	wPoint[0] = wPoint[0] - mPosition[0];
	wPoint[1] = wPoint[1] - mPosition[1];
	wPoint[2] = wPoint[2] - mPosition[2];
	wPoint[3] = 1.0f;
	linAlg::vectorMatrixMult(mOrientation, wPoint, newWPoint);

	wand->getDirection(Dirr);
	linAlg::normVec(Dirr);
	Dirr[3] = 1.0f;
	linAlg::vectorMatrixMult(mOrientation, Dirr, newDirr);
	linAlg::transpose(mOrientation);

	intersection.xyz[0] = newWPoint[0];
	intersection.xyz[1] = newWPoint[1];
	intersection.xyz[2] = newWPoint[2];
	intersection.nxyz[0] = newDirr[0];
	intersection.nxyz[1] = newDirr[1];
	intersection.nxyz[2] = newDirr[2];

}

void Drag::firstSelect(DynamicMesh* mesh, Wand* wand)
{
	float newWPoint[4];
	float Dirr[4]; float newDirr[4];
	float tempVec1[3]; float tempVec2[3];
	float wPoint[4]; float* vPoint; float* vPoint2;
	int index; int index2;

	int tempEdge;

	bool success = false;

	float pLength = 0.0f;
	float oLength = 0.0f;

	float mLength;
	float minLength;

	linAlg::transpose(mOrientation);
	//--< 1.0 | calculated the position and direction of the wand
	wand->getPosition(wPoint);
	wPoint[0] = wPoint[0] - mPosition[0];
	wPoint[1] = wPoint[1] - mPosition[1];
	wPoint[2] = wPoint[2] - mPosition[2];
	wPoint[3] = 1.0f;
	linAlg::vectorMatrixMult(mOrientation, wPoint, newWPoint);

	wand->getDirection(Dirr);
	linAlg::normVec(Dirr);
	Dirr[3] = 1.0f;
	linAlg::vectorMatrixMult(mOrientation, Dirr, newDirr);
	linAlg::transpose(mOrientation);

	intersection.xyz[0] = newWPoint[0];
	intersection.xyz[1] = newWPoint[1];
	intersection.xyz[2] = newWPoint[2];
	intersection.nxyz[0] = newDirr[0];
	intersection.nxyz[1] = newDirr[1];
	intersection.nxyz[2] = newDirr[2];

	// 1.0 >--------------------------
	//--< 2.0 | start searching through vertexarray for points that are within the brush
	for (int i = 0; i <= mesh->vertexCap; i++) {
		//--< 2.1 | calculate vector between vertexposition and wandposition
		vPoint = mVertexArray[i].xyz;
		linAlg::calculateVec(vPoint, newWPoint, tempVec1);
		//tempVec1[0] = vPoint[0] - newWPoint[0];
		//tempVec1[1] = vPoint[1] - newWPoint[1];
		//tempVec1[2] = vPoint[2] - newWPoint[2];
		// 2.1 >---------------------
		mLength = linAlg::vecLength(tempVec1);
		// 2.2 >----------------------
		if (mLength <  radius)
		{
			//--< 2.3 | add the found vertex to list of selected vertices and mark it as selected 
			//changedVertices.push_back(i);
			selectedVertices[selectedSize] = i; selectedSize++;
			//mIndex = sVertsNR;
			//mLength = oLength;

			mVInfoArray[i].selected = 4.0f;
			minLength = mLength;
			mIndex = 0;
			break;
		}
	}

	// 2.3 >-----------------------
	//--< 2.4 | a first vertex has been found, the rest of the search is done through the surface 
	for (int j = 0; j < selectedSize; j++) {
		index2 = selectedVertices[j];
		tempEdge = mVInfoArray[index2].edgePtr;

		do {
			if (mVInfoArray[mEdgeArray[tempEdge].vertex].selected == 0.0f){
				index = mEdgeArray[tempEdge].vertex;
				//changedEdges.push_back(tempEdge);
				//sEdges[nrofsEdges] = tempEdge;
				//nrofsEdges++;
				vPoint2 = mVertexArray[index].xyz;

				tempVec1[0] = vPoint2[0] - newWPoint[0];
				tempVec1[1] = vPoint2[1] - newWPoint[1];
				tempVec1[2] = vPoint2[2] - newWPoint[2];

				mLength = linAlg::vecLength(tempVec1);

				if (mLength < radius) {
					selectedVertices[selectedSize] = index; selectedSize++;
					mVInfoArray[index].selected = 4.0f;
					if (mLength < minLength){
						minLength = mLength;
						mIndex = selectedSize;
					}
				}
			}
			tempEdge = mEdgeArray[mEdgeArray[tempEdge].nextEdge].sibling;

		} while (tempEdge != mVInfoArray[index2].edgePtr);
	}

	index = selectedVertices[0];
	selectedVertices[0] = selectedVertices[mIndex];
	selectedVertices[mIndex] = index;

	lvPoint[0] = mVertexArray[selectedVertices[0]].xyz[0];
	lvPoint[1] = mVertexArray[selectedVertices[0]].xyz[1];
	lvPoint[2] = mVertexArray[selectedVertices[0]].xyz[2];
	lwPoint[0] = newWPoint[0];
	lwPoint[1] = newWPoint[1];
	lwPoint[2] = newWPoint[2];
	// 2.4 >---------------------


}
void Drag::moveVertices(DynamicMesh* mesh, Wand* wand, float dT){

	//deSelect();

	float newWPoint[4];
	float Dirr[4]; float newDirr[4];
	float tempVec1[3]; float tempVec2[3]; float tempVec3[3];
	float wPoint[4]; float* vPoint; float* vPoint2;
	int index; int index2;
	int tempSize;
	float* vNorm;
	float dot;
	float d;
	float l;

	int tempEdge;

	bool success = false;

	float pLength = 0.0f;
	float oLength = 0.0f;

	float mLength;

	tempSize = selectedSize;
	selectedSize = 0;
	//selectedSize = 1;

	linAlg::transpose(mOrientation);

	wand->getPosition(wPoint);
	wPoint[0] = wPoint[0] - mPosition[0];
	wPoint[1] = wPoint[1] - mPosition[1];
	wPoint[2] = wPoint[2] - mPosition[2];
	wPoint[3] = 1.0f;
	linAlg::vectorMatrixMult(mOrientation, wPoint, newWPoint);

	wand->getVelocity(Dirr);
	//linAlg::normVec(Dirr);
	Dirr[3] = 1.0f;
	linAlg::vectorMatrixMult(mOrientation, Dirr, newDirr);
	//linAlg::normVec(newDirr);
	linAlg::transpose(mOrientation);

	tempVec3[0] = newDirr[0];
	tempVec3[1] = newDirr[1];
	tempVec3[2] = newDirr[2];
	linAlg::normVec(tempVec3);
	l = linAlg::vecLength(newDirr);

	intersection.xyz[0] = newWPoint[0];
	intersection.xyz[1] = newWPoint[1];
	intersection.xyz[2] = newWPoint[2];
	intersection.nxyz[0] = newDirr[0];
	intersection.nxyz[1] = newDirr[1];
	intersection.nxyz[2] = newDirr[2];

	/*if (tempSize < 2)
	{
		deSelect();
		firstSelect(mesh, wand);
		return;
	}


	tempVec1[0] = mVertexArray[selectedVertices[0]].xyz[0];
	tempVec1[1] = mVertexArray[selectedVertices[0]].xyz[1];
	tempVec1[2] = mVertexArray[selectedVertices[0]].xyz[2];

	linAlg::calculateVec(newWPoint, lwPoint, tempVec2);

	mVertexArray[selectedVertices[0]].xyz[0] = lvPoint[0] + tempVec2[0];
	mVertexArray[selectedVertices[0]].xyz[1] = lvPoint[1] + tempVec2[1];
	mVertexArray[selectedVertices[0]].xyz[2] = lvPoint[2] + tempVec2[2];

	linAlg::calculateVec(mVertexArray[selectedVertices[0]].xyz, tempVec1, tempVec2);
	tempVec3[0] = tempVec2[0];
	tempVec3[1] = tempVec2[1];
	tempVec3[2] = tempVec2[2];
	linAlg::normVec(tempVec3);
	mVInfoArray[selectedVertices[0]].selected = 3.0f;*/

	//if (mVInfoArray[selectedVertices[0]].edgePtr < 0)
	//	std::cout << "jek";

	for (int i = 0; i < tempSize; i++) {
		index = selectedVertices[i];
		//index = i;

		if (mVInfoArray[index].edgePtr < 0)
			continue;

		vPoint = mVertexArray[index].xyz;
		linAlg::calculateVec(vPoint, newWPoint, tempVec1);

		mLength = linAlg::vecLength(tempVec1);

		if (mLength <  radius)
		{
			selectedVertices[selectedSize] = index; selectedSize++;
			mVInfoArray[index].selected = 3.0f;
			
			vNorm = mVertexArray[index].nxyz;

			tempVec1[0] = (vNorm[0] + tempVec3[0]) / 2.0f;
			tempVec1[1] = (vNorm[1] + tempVec3[1]) / 2.0f;
			tempVec1[2] = (vNorm[2] + tempVec3[2]) / 2.0f;
			linAlg::normVec(tempVec1);
			
			dot = abs(linAlg::dotProd(vNorm, tempVec3));
			//linAlg::normVec(tempVec1);
			mVertexArray[index].xyz[0] += tempVec1[0] * dT * l*dot;
			mVertexArray[index].xyz[1] += tempVec1[1] * dT * l*dot;
			mVertexArray[index].xyz[2] += tempVec1[2] * dT * l*dot;
		}
		else
			mVInfoArray[index].selected = 0.0f;
	}

	if (selectedSize < 2)
	{
		deSelect();
		firstSelect(mesh, wand);
		return;
	}

	//tempVec1[0] = mVertexArray[selectedVertices[0]].xyz[0];
	//tempVec1[1] = mVertexArray[selectedVertices[0]].xyz[1];
	//tempVec1[2] = mVertexArray[selectedVertices[0]].xyz[2];

	//linAlg::calculateVec(newWPoint, lwPoint, tempVec2);

	//mVertexArray[selectedVertices[0]].xyz[0] = lvPoint[0] + tempVec2[0];
	//mVertexArray[selectedVertices[0]].xyz[1] = lvPoint[1] + tempVec2[1];
	//mVertexArray[selectedVertices[0]].xyz[2] = lvPoint[2] + tempVec2[2];

	//linAlg::calculateVec(mVertexArray[selectedVertices[0]].xyz, tempVec1, tempVec2);

	for (int j = 0; j < selectedSize; j++) {
		index2 = selectedVertices[j];
		tempEdge = mVInfoArray[index2].edgePtr;

		do {
			if (mVInfoArray[mEdgeArray[tempEdge].vertex].selected < 3.0f){
				index = mEdgeArray[tempEdge].vertex;

				vPoint2 = mVertexArray[index].xyz;
				linAlg::calculateVec(vPoint2, newWPoint, tempVec1);

				mLength = linAlg::vecLength(tempVec1);

				if (mLength < radius) {
					selectedVertices[selectedSize] = index; selectedSize++;
					mVInfoArray[index].selected = 3.0f;

					vNorm = mVertexArray[index].nxyz;

					tempVec1[0] = (vNorm[0] + tempVec3[0]) / 2.0f;
					tempVec1[1] = (vNorm[1] + tempVec3[1]) / 2.0f;
					tempVec1[2] = (vNorm[2] + tempVec3[2]) / 2.0f;
					linAlg::normVec(tempVec1);

					dot = abs(linAlg::dotProd(vNorm, tempVec3));
					//linAlg::normVec(tempVec1);
					mVertexArray[index].xyz[0] += tempVec1[0] * dT * l * dot;
					mVertexArray[index].xyz[1] += tempVec1[1] * dT * l * dot;
					mVertexArray[index].xyz[2] += tempVec1[2] * dT * l * dot;
				}
			}
			tempEdge = mEdgeArray[mEdgeArray[tempEdge].nextEdge].sibling;

		} while (tempEdge != mVInfoArray[index2].edgePtr);

	}

	mesh->updateArea(selectedVertices, selectedSize);
	mesh->updateOGLData();
}
void Drag::deSelect(){

	for (int i = 0; i < selectedSize; i++)
	{
		mVInfoArray[selectedVertices[i]].selected = 0.0f;
	}
	selectedSize = 0;
}