#pragma once
#include "../glad/glad.h"
#include <vector>

class GPUTextureManager
{

private:

	std::vector<GLuint> m_allotedGLTextures;
	std::vector<GLuint> m_allotedGLSampler;

public:

	GPUTextureManager();
	~GPUTextureManager();

	GLuint createNewTexture(const size_t width, const size_t height, size_t nrChannels, void* data);

	GLuint loadTexture(const char* filePath);

	void destroyTexture(GLuint textureHandle);
	void destroySampler(GLuint samplerHandle);

	GLuint createSampler();
	GLuint getSampler();

};
