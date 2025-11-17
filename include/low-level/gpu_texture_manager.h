#pragma once
#include <unordered_map>


class GPUTextureManager
{

private:

	std::unordered_map<uint64_t, GLuint> m_residentHandleToTextureApiHandle;

public:

	GPUTextureManager();
	~GPUTextureManager();

	TextureInfo createNewTexture(const TextureCreateInfo& textureCreateInfo);
	TextureInfo createBufferTexture(const BufferTextureCreateInfo& bufferTextureCreateInfo);


	//GLuint loadTexture(const char* filePath);

	void destroyTexture(TextureInfo& textureInfo);


};
