#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "mesh.h"
#include "shader.h"

namespace gpr5300
{
	class Model
	{
	public:
		std::vector<Texture> textures_loaded;
		
		void InitModel(const char* path);
		void Draw(const Shader& pipeline) const;
		void Draw(const Shader& pipeline, int amount) const;
		void Delete();
		glm::mat4 model = glm::mat4(1.0);
		std::vector<Mesh> meshes;
	private:
		std::string directory;

		void loadModel(const std::string& path);
		void processNode(const aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			std::string typeName);
	};
}


