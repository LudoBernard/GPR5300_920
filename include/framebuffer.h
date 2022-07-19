#pragma once
#include "shader.h"

namespace gpr5300
{
	class Framebuffer
	{
	public:
		void Init();
		void Delete();
		unsigned int framebuffer;
		unsigned int textureColorbuffer;
		unsigned int vao{}, vbo{}, rbo{};
	private:
		float quadVertices_[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};
		
	};
}

