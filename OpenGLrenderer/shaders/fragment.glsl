#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct PointLight{
	vec3 position;
	vec3 colour;
	float brightness;
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform PointLight light;

uniform vec3 cameraPosition;

in vec3 worldCoords;
in vec3 normal;
in vec3 tangent;
in vec3 binormal;
in vec2 UV;

out vec4 fragmentColour;

void main() {
	vec3 norm = normal;

	float distance = length(light.position - worldCoords);
	float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));

	vec3 ambient = attenuation * light.colour * material.ambient;

	vec3 lightDir = normalize(light.position - worldCoords);
	float diffuseIntensity = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = attenuation * light.brightness * light.colour * diffuseIntensity * material.diffuse;

	vec3 viewDir = normalize(cameraPosition - worldCoords);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0f), material.shininess);
	vec3 specular = attenuation * light.colour * spec * material.specular;

	vec3 result = attenuation*(ambient + diffuse + specular);
	fragmentColour = vec4(result, 1.0f);
}