#version 330 core

uniform vec3 lightColour;
out vec4 fragmentColour;

// For a light source not affected by outside lighting
void main(){
    fragmentColour = vec4(lightColour, 1.0);
}
