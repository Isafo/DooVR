#version 400
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 vertexUV;

out vec2 ssao_uv;
out mat4 projMat;
out vec3 viewNormal

uniform mat4 MV;
uniform mat4 P;

void main () 
{	
	UV = vertexUV;
	projMat = P;
	viewNormal = MV * VertexNormal;

	// Convert position to clip coordinates and pass along to fragment shader
	gl_Position =  (P * MV) * vec4(VertexPosition, 1.0);

}