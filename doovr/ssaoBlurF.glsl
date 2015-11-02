#version 400

uniform sampler2D tex;

uniform int blurSize;

noperspective in vec2 UV;

out float result;

void main ()  {	
	
	vec2 texelSize = 1.0 / vec2(textureSize(tex, 0));
	float result = 0.0;

	vec2 hlim = vec2(float(-blurSize) * 0.5 + 0.5);

	for ( int i = 0; i < blurSize; i++) {
		for (int j = 0; j < blurSize; i++) {
			vec2 offset = (hlim + vec2(float(i), float(j))) * texelSize;
			result += texture(tex, uv + offset).r;
		}
	}

	result = result / float(blurSize * blurSize);
}