#include <fstream>
#include <sstream>
#include <SDL.h>

#include "engine.h"
#include "my_scene.h"

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>



namespace gpr5300
{
	void ModelScene::Begin()
	{
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		framebuffer_.Init();
		skybox_.Init();
		model_.InitModel("data/models/nanosuit/nanosuit.obj");
		sceneShader_.Load("data/shaders/my_scene/my_scene.vert", "data/shaders/my_scene/my_scene.frag");
		simpleColorShader_.Load("data/shaders/my_scene/shader_single_color.vert", "data/shaders/my_scene/shader_single_color.frag");
		framebufferShader_.Load("data/shaders/my_scene/framebuffer.vert", "data/shaders/my_scene/framebuffer.frag");
		skyboxShader_.Load("data/shaders/my_scene/skybox.vert", "data/shaders/my_scene/skybox.frag");


		sceneShader_.Use();

		modelMatrices = new glm::mat4[amount];
		for (unsigned int i = 0; i < amount; i++)
		{
			constexpr float offset = 1.0f;
			glm::mat4 model = glm::mat4(1.0f);
			model = translate(model, glm::vec3(offset*i, 0.0, 0.0));
			model = glm::scale(model, (glm::vec3(0.1, 0.1, 0.1)));
			modelMatrices[i] = model;
		}

		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

		for (unsigned int i = 0; i < model_.meshes.size(); i++)
		{
			unsigned int VAO = model_.meshes[i].vao_;
			glBindVertexArray(VAO);
			// vertex attributes
			std::size_t vec4Size = sizeof(glm::vec4);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			glBindVertexArray(0);
		}
	}

	void ModelScene::End()
	{
		//Unload program/pipeline
		sceneShader_.Delete();
		simpleColorShader_.Delete();
		framebufferShader_.Delete();
		skyboxShader_.Delete();
		framebuffer_.Delete();
		skybox_.Delete();
		model_.Delete();
	}

	void ModelScene::SetLights() const
	{
		sceneShader_.SetVector3("viewPos", camera_.Position);
		sceneShader_.SetVector3("dirLight.direction", camera_.Front);
		sceneShader_.SetVector3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
		sceneShader_.SetVector3("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
		sceneShader_.SetVector3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		sceneShader_.SetInt("material.ambient", 0);
		sceneShader_.SetInt("material.specular", 1);
		sceneShader_.SetInt("material.normal", 2);
		sceneShader_.SetInt("material.texture_shadowMap1", 3);
		sceneShader_.SetFloat("material.shininess", 64.0f);
	}

	void ModelScene::ProcessInput(float dt)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEMOTION) {
				camera_.ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
				break;
			}
		}

		const Uint8* state = SDL_GetKeyboardState(nullptr);

		if (state[SDL_SCANCODE_W]) {
			camera_.ProcessKeyboard(FORWARD, dt);
		}
		if (state[SDL_SCANCODE_A]) {
			camera_.ProcessKeyboard(LEFT, dt);
		}
		if (state[SDL_SCANCODE_S]) {
			camera_.ProcessKeyboard(BACKWARD, dt);
		}
		if (state[SDL_SCANCODE_D]) {
			camera_.ProcessKeyboard(RIGHT, dt);
		}
	}

	void ModelScene::Update(float dt)
	{
		ProcessInput(dt);
		
		//First pass
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_.framebuffer);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		//Draw
		model_.Draw(sceneShader_, amount);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera_.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera_.Zoom), 1280.0f / 720.0f, 0.1f, 100.0f);

		//Set uniforms
		sceneShader_.Use();
		SetLights();
		sceneShader_.SetMatrix4("view", view);
		sceneShader_.SetMatrix4("projection", projection);
		model_.model = rotate(model_.model, dt * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = scale(model_.model, glm::vec3(0.05f, 0.05f, 0.05f)); // it's a bit too big for our scene, so scale it down
		sceneShader_.SetMatrix4("model", model);
		

		model_.Draw(sceneShader_, amount);

		glDepthFunc(GL_LEQUAL);
		skyboxShader_.Use();
		skyboxShader_.SetInt("skybox", 0);
		view = glm::mat4(glm::mat3(camera_.GetViewMatrix()));
		skyboxShader_.SetMatrix4("view", view);
		skyboxShader_.SetMatrix4("projection", projection);
		glBindVertexArray(skybox_.vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_.cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		/*Second pass*/
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		framebufferShader_.Use();
		glBindVertexArray(framebuffer_.vao);
		glDisable(GL_DEPTH_TEST);
		framebufferShader_.SetInt("screenTexture", 0);
		glBindTexture(GL_TEXTURE_2D, framebuffer_.textureColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void ModelScene::Draw()
	{

	}
}

int main(int argc, char** argv)
{
	gpr5300::ModelScene scene;
	gpr5300::Engine engine(&scene);
	engine.Run();
	
	return EXIT_SUCCESS;
}