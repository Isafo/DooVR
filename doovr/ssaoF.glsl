#version 400
layout( location = 0 ) out vec4 FragColor;

in vec3 viewNormal;
in vec2 ssao_uv;
in mat4 projMat;

uniform sampler2D depth_tex;
//uniform sampler2D normal_tex;
//uniform sampler2D noise_tex;

// ssao Uniforms
const int MAX_KERNEL_SIZE = 128;
uniform int kernelSize;
uniform float Radius = 1.5;
uniform float power = 2.0;
uniform vec2 noiseScale; // scale to fit the ssao tex coords to the noise kernel
uniform mat4 sampleKernel[kernelSize];
uniform mat4 noiseKernel[kernelSize];

layout(location=0) out vec4 result;

float ssao(mat3 kernelBasis, vec3 origin, floar Radius) {
	
	float occlusion = 0.0;

	//generate kernel
	for(int i = 0; i < kernelSize; i++) {
		// get sample position in view space
		vec3 sample = kernelBasis * sampleKernel[i];
		sample = sample * Radius + origin;

		// project the sample position back into screen space
		vec4 offset = vec4(sample, 1.0);
		offset = projMat * offset;
		offset.xy /= offset.w;
		offset.xy = offset.xy * 0.5 + 0.5;

		//get sample depth
		float sampleDepth = texture(depth_tex, offset.xy).r;

		// range check and accumulate
		float rangeCheck = abs(origin.z - sampleDepth) < Radius ? 1.0 : 0.0;
		occlusion += (sampleDepth <= sample.z ? 1.0 : 0.0) * rangeCheck;
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
	vec3 rotVec = vec3(noiseKernel[ssao_uv][ssao_uv] * noiseScale) * 2.0 - 1.0;
	//vec3 rotVec = texture(noise_tex, ssao_uv * noiseScale).xyz * 2.0 - 1.0;
	vec3 tangent = normalize(rotvec - viewNormal * dot(rotVec, viewNormal));
	vec3 bitangent = cross(viewNormal, tangent);
	mat3 tbn = mat3(tangent, bitangent, viewNormal);

	result = vec4(ssao(tbn, viewPos, Radius));
}