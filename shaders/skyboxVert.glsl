#version 330 core

layout (location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 camViewMatrix;
uniform mat4 projectionMatrix;

void main(){
    texCoords = position;
    
    // this alters the perspective division of the skybox so that it is always at a depth
    // of 1.0f, which is the maximum depth. This is achieved by setting the z-component
    // equal to the w component; that way z/w = w/w = 1.0f. This ratio, z/w, is the depth
    // value of a fragment.
    vec4 pos = projectionMatrix*camViewMatrix*vec4(position, 1.0f);
    gl_Position = pos.xyww;
}