#pragma once
#include "mesh.h"
#include "model.h"
#include "scene.h"
#include "Shader.h"



namespace gpr5300
{
	class ModelScene final : public Scene
	{
	public:
		void Begin() override;
		void End() override;
		void Update(float dt) override;
		void ProcessInput(float dt);
	private:
		int modelNumber = 0;
		Shader sceneShader_;
		Shader simpleColorShader_;
		Model model_;
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};
}

