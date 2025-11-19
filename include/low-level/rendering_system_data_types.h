#pragma once
#include "../../glad/glad.h"
#include <bitset>


/*VERTEX BUFFER MANAGEMENT SYSTEM*/

enum class BufferUsageType
{
	STATIC,
	DYNAMIC,
	STREAMING,
	UNKNOWN
};


enum class Vertexlayout
{
	INTERLEAVED,
	NON_INTERLEAVED,
	UNKNOWN
};


constexpr size_t MAX_ATTRIBUTES = 15;
using VertexFormat = std::bitset<MAX_ATTRIBUTES>;





enum class VertexAttributeType : std::uint32_t
{
	UNKNOWN				    = 0,
	// Geometric Data
	POSITION_3F				= 1,         // 3 floats for (x, y, z)
	NORMAL_3F				= 2,           // 3 floats for (nx, ny, nz)
	TANGENT_3F				= 3,          // 3 floats for a tangent vector
	BINORMAL_3F				= 4,         // 3 floats for a binormal/bitangent vector

	// Texture
	TEXCOORD_0_2F			= 5,         // 2 floats for (u, v) texture coordinates
	TEXCOORD_1_2F			= 6,
	TEXCOORD_2_2F			= 7,
	TEXCOORD_3_2F			= 8,

	//Color Data
	COLOR_0_4UB_NORMALIZED	= 9, // 4 unsigned bytes for RGBA, treated as [0.0, 1.0]
	COLOR_1_4UB_NORMALIZED = 10,
	COLOR_2_4UB_NORMALIZED = 11,
	COLOR_3_4UB_NORMALIZED = 12,
	// Auxiliary Data
	JOINT_INDICES_4I		= 13,    // 4 integers for skeletal animation joint indices
	JOINT_WEIGHTS_4F		= 14,     // 4 floats for corresponding joint weights

	

};




/*Index Data*/

enum class IndexType
{
	// Unsigned 8-bit integer (1 byte)
		// Max index: 255. Use for small meshes only.
	UBYTE_8, // Maps to GL_UNSIGNED_BYTE (1 byte)

	// Unsigned 16-bit integer (2 bytes)
	// Max index: 65,535. Very common for moderately sized meshes.
	USHORT_16, // Maps to GL_UNSIGNED_SHORT (2 bytes)

	// Unsigned 32-bit integer (4 bytes)
		// Max index: 4,294,967,295. Use for very large meshes.
	UINT_32, // Maps to GL_UNSIGNED_INT (4 bytes)

	UNKNOWN
};


/* VERTEX ATTRIBUTE CONFIGURATION */

struct VertexAttribute
{
	// The type from the VertexAttributeType enum (e.g., POSITION_3F)
	VertexAttributeType type;

	// The location/index in the shader (e.g., layout (location = 0) in ...)
	unsigned int location;

	// The number of components (e.g., 3 for POSITION_3F, 2 for TEXCOORD_2F)
	int count;

	// The OpenGL fundamental type (e.g., GL_FLOAT, GL_UNSIGNED_BYTE)
	unsigned int glType;

	// Whether fixed-point data should be normalized (GL_TRUE/GL_FALSE).
	// Crucial for types like COLOR_4UB_NORMALIZED.
	unsigned char normalized;

	// The size of the attribute in bytes (e.g., 12 bytes for 3 floats)
	size_t size;
};




/* BUFFER SYSTEM */


enum BufferType
{
	VERTEX,
	INDEX,
	SSBO,
	UBO,
	UNKNOWN
};

struct AllocationInfo
{
	size_t offset = 0;

};



struct GPUBufferInfo
{
	BufferType bufferType = BufferType::UNKNOWN;

	GLuint bufferHandle;

	//size in bytes
	size_t size = 0;

	void* mappedPtr = nullptr;

	bool isMapped = false;

};


/*TEXTURE SYSTEM*/




enum class TextureSourcePixelFormat
{
	UNKNOWN,
	R,        // GL_RED
	RG,       // GL_RG
	RGB,      // GL_RGB
	RGBA,     // GL_RGBA
	BGRA      // GL_BGRA (Common in some image formats)
};

// TextureSourceComponentType: Defines the bit size/type of each component in the CPU data.
enum class TextureSourceComponentType
{
	UNKNOWN,
	UNSIGNED_BYTE, // GL_UNSIGNED_BYTE (8-bit per channel)
	FLOAT,         // GL_FLOAT (32-bit float per channel)
	HALF_FLOAT     // GL_HALF_FLOAT (16-bit float per channel)
};




enum class TextureType
{
	TEXTURE_2D,         // GL_TEXTURE_2D
	TEXTURE_3D,         // GL_TEXTURE_3D
	TEXTURE_CUBE_MAP,   // GL_TEXTURE_CUBE_MAP
	TEXTURE_2D_ARRAY,   // GL_TEXTURE_2D_ARRAY
	UNKNOWN
};

enum class TextureInternalFormat
{
	// 8-bit Unsigned Integer Formats
	R8, RG8, RGB8, RGBA8,

	// 16-bit Half-Float Formats
	R16F, RG16F, RGB16F, RGBA16F,

	//  32-bit Full-Float Formats
	R32F, RG32F, RGB32F, RGBA32F,

	// Depth/Stencil formats
	DEPTH_COMPONENT16, DEPTH_COMPONENT24, DEPTH_COMPONENT32F,
	DEPTH_STENCIL,
	UNKNOWN
};

enum class TextureFilter
{
	NEAREST,                // GL_NEAREST
	LINEAR,                 // GL_LINEAR

	NEAREST_MIPMAP_NEAREST, // GL_NEAREST_MIPMAP_NEAREST
	LINEAR_MIPMAP_NEAREST,  // GL_LINEAR_MIPMAP_NEAREST

	NEAREST_MIPMAP_LINEAR,  // GL_NEAREST_MIPMAP_LINEAR
	LINEAR_MIPMAP_LINEAR,    // GL_LINEAR_MIPMAP_LINEAR

	UNKNWON
};

enum class TextureWrap
{
	REPEAT,             // GL_REPEAT
	CLAMP_TO_EDGE,      // GL_CLAMP_TO_EDGE
	MIRRORED_REPEAT,     // GL_MIRRORED_REPEAT
	UNKNOWN
};


struct SamplerSetting
{
	TextureFilter minFilter = TextureFilter::LINEAR;
	TextureFilter magFilter = TextureFilter::LINEAR;

	TextureWrap wrapS = TextureWrap::REPEAT;
	TextureWrap wrapT = TextureWrap::REPEAT;

	TextureWrap wrapR = TextureWrap::REPEAT; // For 3D or Cube Maps
	// Optional: Anisotropy, LOD bias, etc.
};

struct TextureInfo
{
	TextureType type = TextureType::UNKNOWN;
	//GLuint textureHandle = 0;
	uint64_t resisdentHandle = 0;

	int width = 0;
	int height = 0;
	int depth = 0; // For 3D or Array Textures

	TextureInternalFormat internalFormat = TextureInternalFormat::UNKNOWN;
	SamplerSetting samplerSettings;

	bool hasMipmaps = false;
};


struct TextureCreateInfo
{

	TextureType type = TextureType::UNKNOWN;
	// --- Data Source ---
	std::byte* data = nullptr;  // Pointer to the raw pixel data

	// --- Dimensions ---
	int width = 0;
	int height = 0;
	int depth = 1; // Default to 1 for 2D textures

	//--- Sampler Setting ---

	SamplerSetting samplerSetting;


	// --- Format & Type ---
	TextureInternalFormat internalFormat = TextureInternalFormat::UNKNOWN; // GPU side, 

	TextureSourcePixelFormat textureSourcePixelFormat = TextureSourcePixelFormat::RGBA;
	TextureSourceComponentType textureSourceComponentType = TextureSourceComponentType::UNSIGNED_BYTE;
};



struct BufferTextureCreateInfo
{
	// --- Target Type (Crucial for FBOs) ---
	TextureType type = TextureType::TEXTURE_2D;

	// --- Dimensions ---
	int width = 0;
	int height = 0;
	int depth = 1;

	// --- Format (The ONLY data format needed) ---
	TextureInternalFormat textureInternalFormat = TextureInternalFormat::UNKNOWN;

	// --- Mipmaps ---
	//useful for G-Buffer textures
	// that might be used later for mipmap generation.
	bool generateMipmaps = false;

	// --- Optional Storage Flag ---
	// If we have plan to use this in a Compute Shader as a GL_IMAGE_... (read/write access)
	bool useAsStorage = false;
};