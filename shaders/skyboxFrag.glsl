#version 330 core

in vec3 texCoords;
out vec4 fragmentColour;
uniform samplerCube skybox;


void main(){
	fragmentColour = texture(skybox, texCoords);

	// optional gamma correction
	float gamma = 2.2f;
	fragmentColour = vec4(pow(fragmentColour.rgb, vec3(1.0f/gamma)), 1.0f);
}