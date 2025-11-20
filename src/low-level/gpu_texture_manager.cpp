

#include <stdexcept>
#include <cassert>

#include "../../include/low-level/rendering_system_data_types.h"
#include "../../include/low-level/gpu_texture_manager.h"
#include "../../glad/glad.h"




namespace TextureUtils
{

	GLenum toGLEnum(TextureSourcePixelFormat format)
	{
		switch (format)
		{
		case TextureSourcePixelFormat::R:     return GL_RED;
		case TextureSourcePixelFormat::RG:    return GL_RG;
		case TextureSourcePixelFormat::RGB:   return GL_RGB;
		case TextureSourcePixelFormat::RGBA:  return GL_RGBA;
		case TextureSourcePixelFormat::BGRA:  return GL_BGRA;
		default: return 0; // Handle error or unknown
		}
	}

	GLenum toGLEnum(TextureSourceComponentType type)
	{
		switch (type)
		{
		case TextureSourceComponentType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
		case TextureSourceComponentType::FLOAT:         return GL_FLOAT;
		case TextureSourceComponentType::HALF_FLOAT:    return GL_HALF_FLOAT;
		default: return 0;
		}
	}




	GLenum toGLEnum(TextureType type)
	{
		switch (type)
		{
		case TextureType::TEXTURE_2D:       return GL_TEXTURE_2D;
		case TextureType::TEXTURE_3D:       return GL_TEXTURE_3D;
		case TextureType::TEXTURE_CUBE_MAP: return GL_TEXTURE_CUBE_MAP;
		case TextureType::TEXTURE_2D_ARRAY: return GL_TEXTURE_2D_ARRAY;
		default: return 0; // Unknown or error type
		}
	}


	//------- SAMPLER --------//


	GLenum toGLEnum(TextureFilter filter, bool isMagFilter = false)
	{
		if (isMagFilter)
		{
			// Magnification filter cannot use mipmapping options
			switch (filter)
			{
			case TextureFilter::NEAREST:
			case TextureFilter::NEAREST_MIPMAP_NEAREST:
			case TextureFilter::NEAREST_MIPMAP_LINEAR:
				return GL_NEAREST; // Mag filter must be GL_NEAREST or GL_LINEAR

			case TextureFilter::LINEAR:
			case TextureFilter::LINEAR_MIPMAP_NEAREST:
			case TextureFilter::LINEAR_MIPMAP_LINEAR:
				return GL_LINEAR; // Mag filter must be GL_NEAREST or GL_LINEAR

			default: return GL_LINEAR; // Default safe value
			}
		}
		else
		{
			// Minification filter uses the full range of options
			switch (filter)
			{
			case TextureFilter::NEAREST: 				return GL_NEAREST;
			case TextureFilter::LINEAR: 				return GL_LINEAR;
			case TextureFilter::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
			case TextureFilter::LINEAR_MIPMAP_NEAREST: 	return GL_LINEAR_MIPMAP_NEAREST;
			case TextureFilter::NEAREST_MIPMAP_LINEAR: 	return GL_NEAREST_MIPMAP_LINEAR;
			case TextureFilter::LINEAR_MIPMAP_LINEAR: 	return GL_LINEAR_MIPMAP_LINEAR;
			default: return GL_LINEAR_MIPMAP_LINEAR;
			}

		}
	}

	GLenum toGLEnum(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::REPEAT: 			return GL_REPEAT;
		case TextureWrap::CLAMP_TO_EDGE: 	return GL_CLAMP_TO_EDGE;
		case TextureWrap::MIRRORED_REPEAT: 	return GL_MIRRORED_REPEAT;
			// You could add CLAMP_TO_BORDER here if needed
		default: return GL_REPEAT; // Default safe value
		}
	}

	bool requiresMipmaps(TextureFilter minFilter)
	{
		return (minFilter == TextureFilter::NEAREST_MIPMAP_NEAREST ||
			minFilter == TextureFilter::LINEAR_MIPMAP_NEAREST ||
			minFilter == TextureFilter::NEAREST_MIPMAP_LINEAR ||
			minFilter == TextureFilter::LINEAR_MIPMAP_LINEAR);
	}



}







GPUTextureManager::GPUTextureManager()
{


}

GPUTextureManager::~GPUTextureManager()
{
	//destroying texture handles
		//glDeleteTextures(m_allotedGLTextures.size(), m_allotedGLTextures.data());
	

	//TODO : destroy any more new type of resources also


}


TextureInfo GPUTextureManager::createNewTexture(const TextureCreateInfo& textureCreateInfo)
{



	GLenum glInternalFormat = (GLenum)textureCreateInfo.internalFormat;
	GLenum glSourceFormat = TextureUtils::toGLEnum(textureCreateInfo.textureSourcePixelFormat);
	GLenum glSourceType = TextureUtils::toGLEnum(textureCreateInfo.textureSourceComponentType);



	
	GLuint glTextureId;
	glCreateTextures(GL_TEXTURE_2D, 1, &glTextureId);


	// Special handling for 1-channel textures (like R/Luminance) to prevent padding issues
	if (textureCreateInfo.textureSourcePixelFormat == TextureSourcePixelFormat::R)
	{
		// OpenGL defaults to 4-byte alignment; this forces 1-byte alignment for R-only data.
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}
	else
	{
		// Restore default alignment for multi-channel data
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}


	GLenum target = TextureUtils::toGLEnum(textureCreateInfo.type);

	SamplerSetting samplerSettings = textureCreateInfo.samplerSetting;



	// 4. **Upload Data (The core GL call)**
	// This single call handles all formats/types using the mapped enums
	glTexImage2D(
		target,
		0,                                 // Mipmap level
		glInternalFormat,                  // e.g., GL_RGBA8 (GPU Storage)
		textureCreateInfo.width,
		textureCreateInfo.height,
		0,                                 // Border (must be 0)
		glSourceFormat,                    // e.g., GL_RGB (CPU Data Layout)
		glSourceType,                      // e.g., GL_UNSIGNED_BYTE (CPU Component Type)
		textureCreateInfo.data             // Raw pixel data
	);

	// 5. **Apply Sampler Settings**
	// Apply filtering and wrapping modes based on the SAMPLER struct
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, TextureUtils::toGLEnum(samplerSettings.minFilter));
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, TextureUtils::toGLEnum(samplerSettings.magFilter));
	glTexParameteri(target, GL_TEXTURE_WRAP_S, TextureUtils::toGLEnum(samplerSettings.wrapS));
	glTexParameteri(target, GL_TEXTURE_WRAP_T, TextureUtils::toGLEnum(samplerSettings.wrapT));
	// Note: If you have a wrapR setting, you'd use it here for 3D textures/cubemaps

	// 6. **Mipmap Generation (Conditional)**
	bool hasMipmaps = TextureUtils::requiresMipmaps(samplerSettings.minFilter);

	if (hasMipmaps)
	{
		glGenerateMipmap(target);
	}

	// 7. **Bindless Handle (Optional)**
	GLuint64 residentHandle = 0;
	// Assuming you have a global flag/setting to enable bindless

		residentHandle = glGetTextureHandleARB(glTextureId);
		glMakeTextureHandleResidentARB(residentHandle);
	

	// 8. **Store and Return Metadata**
	//m_allotedGLTextureIDs.push_back(glTextureId); // For destruction tracking




	TextureInfo info;
	//info.textureHandle = glTextureId;
	info.resisdentHandle = residentHandle;
	info.type = textureCreateInfo.type;
	info.width = textureCreateInfo.width;
	info.height = textureCreateInfo.height;
	info.internalFormat = textureCreateInfo.internalFormat;
	info.samplerSettings = samplerSettings;
	info.hasMipmaps = hasMipmaps;

	// You would map the info via the residentHandle in m_textureRegistry here
	m_residentHandleToTextureApiHandle.emplace(residentHandle, glTextureId);// info);

	return info;

}

TextureInfo GPUTextureManager::createBufferTexture(const BufferTextureCreateInfo& bufferTextureCreateInfo)
{




	return TextureInfo();
}



void GPUTextureManager::destroyTexture(TextureInfo& textureInfo)
{

	const uint64_t residentHandle = textureInfo.resisdentHandle;
	auto it = m_residentHandleToTextureApiHandle.find(residentHandle);

	if (it == m_residentHandleToTextureApiHandle.end())
	{
		// Handle error: texture not found
		return;
	}

	GLuint glTextureId = it->second;

	// 2. Make the 64-bit handle non-resident
	glMakeTextureHandleNonResidentARB(residentHandle);


	glDeleteTextures(1, &glTextureId);


	m_residentHandleToTextureApiHandle.erase(it);


}


