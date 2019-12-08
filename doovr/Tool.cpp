#include "Tool.h"


Tool::Tool()
	: toolBrush(nullptr),
	selectedVertices(nullptr),
	selectedSize(0.0f),
	MAX_SELECTED(100000)
{
}


Tool::~Tool()
{
}
