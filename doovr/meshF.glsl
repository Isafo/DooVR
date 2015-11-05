#version 400
layout( location = 0 ) out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec4 uv;
in vec4 shadowUV;

uniform vec4 lightPos;
uniform vec4 lightPos2;
uniform int blurSize;


uniform sampler2D dTex;
uniform sampler2D ssao_tex;

uniform float Radius;

float blurSSAO() {
	vec2 texelSize = 1.0 / vec2(textureSize(ssao_tex, 0));
	float result = 0.0;

	vec2 hlim = vec2(float(-blurSize) * 0.5 + 0.5);

	for ( int i = 0; i < blurSize; i++) {
		for (int j = 0; j < blurSize; i++) {
			vec2 offset = (hlim + vec2(float(i), float(j))) * texelSize;
			result += texture(ssao_tex, uv.st + offset).r;
		}
	}

	result = result / float(blurSize * blurSize);
	return result;
}


void main () {

	float occlusion = blurSSAO();

	vec3 LightIntensity;

	if( ( (uv.z > Radius) && (uv.z < Radius + 0.001f) && texture( dTex, shadowUV.xy ).z  >  shadowUV.z - 0.001) && (uv.w < Radius + 0.001f) && (uv.w > -(Radius + 0.001f)) ){	
		LightIntensity = vec3(1.0f, 1.0f, 1.0f);
	} 
	else if ( ( (uv.w < -Radius) && (uv.w > -(Radius + 0.001f)) || (uv.w > Radius) && (uv.w < Radius + 0.001f) ) && (uv.z < Radius))  {
		LightIntensity = vec3(1.0f, 1.0f, 1.0f);
	}
	else {
		LightIntensity = vec3(0.3f, 0.02f, 0.0f) * occlusion;
	}
	
	vec3 Kd = vec3(0.7f, 0.7f, 0.7f);                // Diffuse reflectivity
	vec3 Ka = vec3(0.1f, 0.1f, 0.1f);                // Ambient reflectivity
	vec3 Ks = vec3( 0.2f, 0.2f, 0.2f);				 // Specular reflectivity
	float Shininess = 6.0f;						 // Specular shininess factor
	vec3 norm = normalize( Normal );			
	vec3 vie = normalize(vec3(-Position));			 // viewDir	
		
	float strength = 0.8f;

	//float distance0 = length( vec3(lPos) - Position);
	
	// 0th
	vec3 s = normalize( vec3(lightPos) - Position ); // lightDir
	vec3 r = reflect( -s, norm );						 // reflectDir
	//vec3 halfwayDir = normalize(lightDir + viewDir);  
	//float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
	vec3 LI = LightIntensity * (  Ka + Kd * max( dot(s, norm), 0.0 )) + Ks * pow( max( dot(r,vie), 0.0 ), Shininess ) * strength;

	vec3 s2 = normalize( vec3(lightPos2) - Position ); // lightDir
	vec3 r2 = reflect( -s2, norm );						 // reflectDir
	vec3 LI2 = LightIntensity * (  Ka + Kd * max( dot(s2, norm), 0.0 ))  + Ks * pow( max( dot(r2,vie), 0.0 ), Shininess ) * strength;
	
	vec3 newResultLight = (LI + LI2 )/2.0f;
	FragColor = vec4(newResultLight, 1.0);
}