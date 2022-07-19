#pragma once

class ShadowBuffer
{
public:
		void InitSB();
		static void bindDefaultBuffer();

		unsigned int texture_;

		unsigned int textureShadowbuffer_;
		unsigned int svao_{}, svbo_{}, sfbo_{}, srbo_{};
		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
};