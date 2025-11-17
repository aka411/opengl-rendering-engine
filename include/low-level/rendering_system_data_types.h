#pragma once
#include "../../glad/glad.h"


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

enum class VertexAttributeType
{
	// Geometric Data
	POSITION_3F,         // 3 floats for (x, y, z)
	NORMAL_3F,           // 3 floats for (nx, ny, nz)
	TANGENT_3F,          // 3 floats for a tangent vector
	BINORMAL_3F,         // 3 floats for a binormal/bitangent vector

	// Texture and Color Data
	TEXCOORD_2F,         // 2 floats for (u, v) texture coordinates
	COLOR_4UB_NORMALIZED, // 4 unsigned bytes for RGBA, treated as [0.0, 1.0]

	// Auxiliary Data
	JOINT_INDICES_4I,    // 4 integers for skeletal animation joint indices
	JOINT_WEIGHTS_4F,     // 4 floats for corresponding joint weights

	UNKNOWN

};




enum class VertexFormat
{
	// Common Formats (P = Position, N = Normal, T = Texture Coords)

	// P3f
	POSITION,

	// P3f_N3f
	POSITION_NORMAL,

	// P3f_T2f
	POSITION_TEXCOORD,

	// P3f_N3f_T2f
	POSITION_NORMAL_TEXCOORD,

	// P3f_N3f_T2f_C4ub
	POSITION_NORMAL_TEXCOORD_COLOR,

	// P3f_N3f_T2f_Tan3f
	POSITION_NORMAL_TEXCOORD_TANGENT, // Used for tangent space calculations (Normal Mapping)

	// P3f_N3f_T2f_J4i_W4f (Animated Mesh Format)
	//SKINNED_MESH
	POSITION_NORMAL_TEXCOORD_TANGENT_JOINT_WEIGHT,


    UNKNWON
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




enum class TextureSourceFormat
{
	UNKNOWN,
	R,        // GL_RED
	RG,       // GL_RG
	RGB,      // GL_RGB
	RGBA,     // GL_RGBA
	BGRA      // GL_BGRA (Common in some image formats)
};

// TextureSourceType: Defines the bit size/type of each component in the CPU data.
enum class TextureSourceType
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

enum class TextureFormat
{
	// Internal formats
	R8, RG8, RGB8, RGBA8,

	R16F, RG16F, RGB16F, RGBA16F,

	// Depth/Stencil formats
	DEPTH_COMPONENT,//TODO : correct to add bit depth
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

	TextureFormat internalFormat = TextureFormat::UNKNOWN;
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
	TextureFormat internalFormat = TextureFormat::UNKNOWN; // Still your GPU-side enum (e.g., GL_RGBA8)

	TextureSourceFormat sourceFormat = TextureSourceFormat::RGBA;
	TextureSourceType sourceType = TextureSourceType::UNSIGNED_BYTE;
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
	TextureFormat internalFormat = TextureFormat::UNKNOWN;

	// --- Mipmaps ---
	//useful for G-Buffer textures
	// that might be used later for mipmap generation.
	bool generateMipmaps = false;

	// --- Optional Storage Flag ---
	// If we have plan to use this in a Compute Shader as a GL_IMAGE_... (read/write access)
	bool useAsStorage = false;
};