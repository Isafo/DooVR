#version 400
// Ouput data
layout(location = 0) out vec4 FragColor;

void main() {
   FragColor = vec4(gl_PrimitiveID + 1, 1.0f, 1.0f, 1.0f);
}