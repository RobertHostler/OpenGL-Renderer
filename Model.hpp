#pragma once


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <string>
#include "stb_image.h"
#include "./Mesh.hpp"
#include "./Shader.hpp"


class Model{
public:
	std::vector<Mesh> meshes;

	Model(std::string path);
	void Draw(Shader &shader);
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};