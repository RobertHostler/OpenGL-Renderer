#version 330 core

in vec3 texCoords;
//in vec3 Position;
//in vec3 Normal;

out vec4 fragmentColour;

uniform samplerCube skybox;
//uniform vec3 camPos;

void main(){
  // sample the skybox using reflection effects
  // NOTE: this should be used on a separate cube object, NOT on the skybox itself
  // vec3 I = normalize(Position - camPos);
  // vec3 R = reflect(I, normalize(Normal));
  // fragmentColour = vec4(texture(skybox, R).rgb, 1.0f);

  // ordinary skybox sampling; no reflection or refraction
  fragmentColour = texture(skybox, texCoords);
  //fragmentColour = 0.01f*texture(skybox, texCoords) + vec4(1.0f, 0.0f, 0.0f, 1.0f);

  // optional gamma correction
  float gamma = 2.2f;
  fragmentColour = vec4(pow(fragmentColour.rgb, vec3(1.0f/gamma)), 1.0f);
}