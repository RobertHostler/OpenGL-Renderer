#version 330 core

layout (location = 0) in vec3 positionAttribute;
layout (location = 1) in vec3 normalAttribute;
layout (location = 2) in vec3 tangentAttribute;
layout (location = 3) in vec2 textureUV;

out vec3 worldCoords;
out vec3 normal;
out vec3 tangent;
out vec3 binormal;
out vec2 UV;

uniform mat4 modelMatrix;
uniform mat4 camViewMatrix;
uniform mat4 projectionMatrix;

void main() {
	gl_Position = projectionMatrix*camViewMatrix*modelMatrix*vec4(positionAttribute, 1.0);
	worldCoords = (modelMatrix*vec4(positionAttribute, 1.0)).xyz;

	// Update the TNB frame to account for model rotation
	normal   = normalize((modelMatrix * vec4(normalAttribute, 0.0)).xyz);
	tangent  = normalize((modelMatrix * vec4(tangentAttribute, 0.0)).xyz);
	binormal = cross(tangent, normal);

	UV = textureUV;
}