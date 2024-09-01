#ifndef SKYBOX_H
#define SKYBOX_H


#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

#include "./Shader.hpp"


class Skybox{
public:

	const float skyboxVertices[24] = {
	    -1.0f,  1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f,  1.0f, -1.0f,

	    -1.0f,  1.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,
	     1.0f, -1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f
	};

	const unsigned int skyboxIndices[36] = {
		0, 1, 2,
		0, 2, 3,

		3, 2, 6,
		3, 6, 7,

		7, 6, 5,
		7, 5, 4,

		4, 5, 1,
		4, 1, 0,

		4, 0, 3,
		4, 3, 7,

		1, 5, 6,
		1, 6, 2
	};

	unsigned int cubemapID;
	unsigned int skyboxVBO;
	unsigned int skyboxVAO;
	unsigned int skyboxIBO;

	Shader skyboxShader;

	Skybox(
		const char *pathRight, const char *pathLeft,
		const char *pathTop, const char *pathBottom,
		const char *pathFront, const char *pathBack
	);

	void Draw(glm::mat4 camViewMat, glm::mat4 projMat);
};


#endif