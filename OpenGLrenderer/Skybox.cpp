#include "./Skybox.hpp"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Skybox::Skybox(
const char *pathRight, const char *pathLeft,
const char *pathTop, const char *pathBottom,
const char *pathFront, const char *pathBack) : skyboxShader("./shaders/skyboxVert.glsl", "./shaders/skyboxFrag.glsl") {

	glGenTextures(1, &cubemapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);

	int width, height, numChannels;
	unsigned char* cubemapData;
	
	cubemapData = stbi_load(pathRight, &width, &height, &numChannels, 0);
	if(cubemapData){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapData);
		stbi_image_free(cubemapData);
	} else {
		std::cout << "the cubemap '" << pathRight << "' failed to load" << std::endl;
		stbi_image_free(cubemapData);
	}

	cubemapData = stbi_load(pathLeft, &width, &height, &numChannels, 0);
	if(cubemapData){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapData);
		stbi_image_free(cubemapData);
	} else {
		std::cout << "the cubemap 'pathLeft' failed to load" << std::endl;
		stbi_image_free(cubemapData);
	}

	cubemapData = stbi_load(pathTop, &width, &height, &numChannels, 0);
	if(cubemapData){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapData);
		stbi_image_free(cubemapData);
	} else {
		std::cout << "the cubemap './skybox/top.jpg' failed to load" << std::endl;
		stbi_image_free(cubemapData);
	}
	
	cubemapData = stbi_load(pathBottom, &width, &height, &numChannels, 0);
	if(cubemapData){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapData);
		stbi_image_free(cubemapData);
	} else {
		std::cout << "the cubemap './skybox/bottom.jpg' failed to load" << std::endl;
		stbi_image_free(cubemapData);
	}
	
	cubemapData = stbi_load(pathFront, &width, &height, &numChannels, 0);
	if(cubemapData){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapData);
		stbi_image_free(cubemapData);
	} else {
		std::cout << "the cubemap './skybox/front.jpg' failed to load" << std::endl;
		stbi_image_free(cubemapData);
	}
	
	cubemapData = stbi_load(pathBack, &width, &height, &numChannels, 0);
	if(cubemapData){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemapData);
		stbi_image_free(cubemapData);
	} else {
		std::cout << "the cubemap './skybox/back.jpg' failed to load" << std::endl;
		stbi_image_free(cubemapData);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	skyboxShader.UseShader();
	skyboxShader.SetUniform1i("skybox", 0);

	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void Skybox::Draw(glm::mat4 camViewMat, glm::mat4 projMat){
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	skyboxShader.UseShader();
	skyboxShader.SetUniformMatrix4fv("camViewMatrix", camViewMat);
	skyboxShader.SetUniformMatrix4fv("projectionMatrix", projMat);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);
}