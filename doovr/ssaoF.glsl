#version 400
layout( location = 0 ) out vec4 FragColor;

in vec3 viewNormal;
in vec2 ssao_uv;
in mat4 projMat;

uniform sampler2D depth_tex;
uniform sampler2D noise_tex;

// ssao Uniforms
const int MAX_KERNEL_SIZE = 16;
uniform int kernelSize;
uniform float Radius = 1.5;
uniform float power = 2.0;
uniform vec2 noiseScale; // scale to fit the ssao tex coords to the noise kernel
uniform vec3 sampleKernel[MAX_KERNEL_SIZE];

layout(location=0) out vec4 result;

float ssao(mat3 kernelBasis, vec3 origin, float radius) {
	
	float occlusion = 0.0;

	//generate kernel
	for(int i = 0; i < kernelSize; i++) {
		// get sample position in view space
		vec3 samplePos = kernelBasis * sampleKernel[i];
		samplePos = samplePos * radius + origin;

		// project the samplePos position back into screen space
		vec4 offset = vec4(samplePos, 1.0);
		offset = projMat * offset;
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5 + 0.5;

		//get sample depth
		float sampleDepth = texture(depth_tex, offset.xy).r;

		// range check and accumulate
		float rangeCheck = abs(origin.z - sampleDepth) < radius ? 1.0 : 0.0;
		occlusion += (sampleDepth <= samplePos.z ? 1.0 : 0.0) * rangeCheck;
	}

	// normalize and invert the occlusion
	return occlusion = 1.0 - (occlusion / kernelSize); 
}


void main () {
	// calculate view space position
	float x = ssao_uv.s * 2.0 - 1.0;
	float y = ssao_uv.t * 2.0 - 1.0;

	float z = texture(depth_tex, ssao_uv).r * 2.0 - 1.0;

	vec4 projPos = vec4(x, y, z, 1.0);
	vec4 viewPos = inverse(projMat) * projPos;

	viewPos /= viewPos.w;

	// calculate the change-of-bias mat
	vec3 rotVec = texture(noise_tex, ssao_uv * noiseScale).xyz * 2.0 - 1.0;
	vec3 tangent = normalize(rotVec - viewNormal * dot(rotVec, viewNormal));
	vec3 bitangent = cross(viewNormal, tangent);
	mat3 tbn = mat3(tangent, bitangent, viewNormal);

	result = vec4(ssao(tbn, vec3(viewPos), Radius));
}