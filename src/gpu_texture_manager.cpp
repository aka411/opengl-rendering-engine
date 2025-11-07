#include "../include/gpu_texture_manager.h"
#include "../include/stb_image.h"
#include <stdexcept>
#include <cassert>

GPUTextureManager::GPUTextureManager()
{
	createSampler();

}

GPUTextureManager::~GPUTextureManager()
{
	//destroying texture handles
		glDeleteTextures(m_allotedGLTextures.size(), m_allotedGLTextures.data());
	

	//TODO : destroy any more new type of resources also


}


GLuint GPUTextureManager::loadTexture(const char* filePath)
{
	//use stb to load file and get image data

	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data == nullptr)
	{
		throw std::runtime_error("stb failed to laod image data check file path ");

	}

	GLuint glTextureId = createNewTexture(width, height,nrChannels, data);
	

	stbi_image_free(data);

	return glTextureId;
}

void GPUTextureManager::destroyTexture(GLuint textureHandle)
{
	//Not going to destroy anthing this manager does not own
	//though i do need more thought on this
	for (size_t i = 0; i < m_allotedGLTextures.size(); ++i)
	{
		if (m_allotedGLTextures[i] == textureHandle)
		{
			glDeleteTextures(1,  &textureHandle);
			m_allotedGLTextures[i] = m_allotedGLTextures.back();
			m_allotedGLTextures.pop_back();
			break;
		}
	}
}

void GPUTextureManager::destroySampler(GLuint samplerHandle)
{
	for (size_t i = 0; i < m_allotedGLSampler.size(); ++i)
	{
		if (m_allotedGLSampler[i] == samplerHandle)
		{
			glDeleteSamplers(1, &samplerHandle);
			m_allotedGLSampler[i] = m_allotedGLSampler.back();
			m_allotedGLSampler.pop_back();
			break;
		}
	}
}

GLuint GPUTextureManager::createSampler()
{
	GLuint samplerHandle;

	glGenSamplers(1, &samplerHandle);

	glSamplerParameteri(samplerHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set texture wrapping mode to clamp to the edges
	// This is important to prevent artifacts when sampling near the edge of a glyph.
	glSamplerParameteri(samplerHandle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(samplerHandle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	m_allotedGLSampler.push_back(samplerHandle);

	return samplerHandle;
}


GLuint GPUTextureManager::getSampler()
{
	assert(m_allotedGLSampler.size() > 0 && "No sampler created yet");
	return 	m_allotedGLSampler[0];

}



GLuint GPUTextureManager::createNewTexture(const size_t width, const size_t height,size_t nrChannels, void* data)
{
	//TODO : later add a way to give format also 

	GLuint glTextureId;
	glGenTextures(1, &glTextureId);
	glBindTexture(GL_TEXTURE_2D, glTextureId);
	if (nrChannels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if (nrChannels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else if (nrChannels == 1)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
	}

	m_allotedGLTextures.push_back(glTextureId);//To facilate destruction when manager exits

	return glTextureId;

}


