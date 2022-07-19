#pragma once
#include "mesh.h"
#include "model.h"
#include "scene.h"
#include "shader.h"
#include "framebuffer.h"
#include "cubemap.h"
#include "camera.h"
#include "shadowbuffer.h"

namespace gpr5300
{
	class ModelScene final : public Scene
	{
	public:
		void Begin() override;
		void End() override;
		void SetLights() const;
		void ProcessInput(float dt);
		void Update(float dt) override;
		void Draw();
	private:
		Framebuffer framebuffer_;
		Shader sceneShader_;
		Shader simpleColorShader_;
		Shader framebufferShader_;
		Shader skyboxShader_;
		Cubemap skybox_;
		Model model_;
		Camera camera_{glm::vec3(0.0f, 0.0f, 3.0f)};
		unsigned int amount = 25;
		glm::mat4* modelMatrices;
	};
}

