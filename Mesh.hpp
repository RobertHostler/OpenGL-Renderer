#pragma once


#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "./Shader.hpp"


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 uv;
};


class Mesh {
public:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void Draw(Shader &shader);

	void setupBuffers();
};