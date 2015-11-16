#version 400
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 MV;
uniform mat4 P;

void main () {	
	// Convert position to clip coordinates and pass along to fragment shader
	gl_Position =  MV * vec4(VertexPosition, 1.0);
}