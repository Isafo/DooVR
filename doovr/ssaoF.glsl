#version 400
layout( location = 0 ) out vec4 FragColor;

in vec3 viewNormal;
in vec2 ssao_uv;
in mat4 projMat;
in vec3 viewPos;

uniform sampler2D depth_tex;
uniform sampler2D noise_tex;

// ssao Uniforms
const int MAX_KERNEL_SIZE = 16;
uniform int kernelSize;
uniform float radius = 1.5;
uniform float power = 2.0;
uniform vec2 noiseScale; // scale to fit the ssao tex coords to the noise kernel
uniform vec3 sampleKernel[MAX_KERNEL_SIZE];

const int sample_count = 16;
const vec2 poisson16[] = vec2[](    // These are the Poisson Disk Samples
                                vec2( -0.94201624,  -0.39906216 ),
                                vec2(  0.94558609,  -0.76890725 ),
                                vec2( -0.094184101, -0.92938870 ),
                                vec2(  0.34495938,   0.29387760 ),
                                vec2( -0.91588581,   0.45771432 ),
                                vec2( -0.81544232,  -0.87912464 ),
                                vec2( -0.38277543,   0.27676845 ),
                                vec2(  0.97484398,   0.75648379 ),
                                vec2(  0.44323325,  -0.97511554 ),
                                vec2(  0.53742981,  -0.47373420 ),
                                vec2( -0.26496911,  -0.41893023 ),
                                vec2(  0.79197514,   0.19090188 ),
                                vec2( -0.24188840,   0.99706507 ),
                                vec2( -0.81409955,   0.91437590 ),
                                vec2(  0.19984126,   0.78641367 ),
                                vec2(  0.14383161,  -0.14100790 )
                               );

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


vec3 calcPos(vec2 coord, float depth) {
	return vec3(coord, depth);
}

void main () {
	// calculate view space position
	/*float x = ssao_uv.s * 2.0 - 1.0;
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

	result = vec4(ssao(tbn, vec3(viewPos), radius));*/

	float occlusion = 0.0f;

	for (int i = 0; i < sample_count; i++) {
		vec2 sampleTexCoord = ssao_uv + (poisson16[i] * radius);
		float sampleDepth = texture(depth_tex, sampleTexCoord).r;
		vec3 samplePos = calcPos(sampleTexCoord, sampleDepth * 2 - 1);
		vec3 sampleDir = normalize(samplePos - viewPos);

		// calculate the angle between sampleDir and the fragments normal
		// clamp the values between 0-1 we do not want contribution form negative angles
		float dotProd = max(dot(viewNormal, sampleDir), 0);

		// calculate the distance between the fragment and the samplePos
		float dist = length(samplePos - viewPos);
		
		// radius should be the distance threshold
		float distFunc = 1.0 - smoothstep(radius, radius * 2, dist);
		occlusion += (distFunc * dotProd);
	}

	FragColor.a = 1.0 - (occlusion / sample_count);
}