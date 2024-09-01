#include "./Model.hpp"


void Model::Draw(Shader &shader){
	for(unsigned int i = 0; i < meshes.size(); i++){
		meshes[i].Draw(shader);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for(unsigned int i = 0; i < mesh -> mNumVertices; i++){
		Vertex vertex;

		// load the vertices
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		vertex.position = position;

		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.normal = normal;

		glm::vec3 tangent;
		tangent.x = mesh->mTangents[i].x;
		tangent.y = mesh->mTangents[i].y;
		tangent.z = mesh->mTangents[i].z;
		vertex.tangent = tangent;

		// UV coordinates
		if(mesh -> mTextureCoords[0]){
			glm::vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vertex.uv = uv;
		} else {
			vertex.uv = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++){
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices);
}

void Model::processNode(aiNode *node, const aiScene *scene){
	for(unsigned int i = 0; i < node->mNumMeshes; i++){
		aiMesh *mesh = scene->mMeshes[ node->mMeshes[i] ];
		meshes.push_back(processMesh(mesh, scene));
	}
	for(unsigned int i = 0; i < node->mNumChildren; i++){
		processNode(node->mChildren[i], scene);
	}
}

Model::Model(std::string path){
	Assimp::Importer import;

	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if(!scene || scene -> mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene -> mRootNode){
		std::cout << "Assimp Failed to read the file: " << path << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}