#pragma once

#include <vector>
#include <glm/ext/quaternion_float.hpp>
#include "../glad/glad.h"
#include <map>
#include <string>
#include <unordered_map>

namespace Engine
{


	/*ANIMATION*/
	enum class AnimationInterpolationMode
	{
		LINEAR,
		STEP,
		CUBICSPLINE,
		UNKNOWN
	};

	enum class AnimationPathType
	{
		TRANSLATION,
		ROTATION,
		SCALE,
		UNKNOWN
	};


	/**
	struct AnimationTranslationSampler
	{
		AnimationInterpolationMode interpolationMode;
		std::vector<float> inputKeyframes; //time
		std::vector<glm::vec3> translations; //can be vec3 or vec4 depending on path type
	};

	struct AnimationRotationSampler
	{
		AnimationInterpolationMode interpolationMode;
		std::vector<float> inputKeyframes; //time
		std::vector<glm::quat> rotations; //can be vec3 or vec4 depending on path type
	};

	struct AnimationScaleSampler
	{
		AnimationInterpolationMode interpolationMode;
		std::vector<float> inputKeyframes; //time
		std::vector<glm::vec3> scales; //can be vec3 or vec4 depending on path type
	};
	*/



	struct AnimationTimeInput
	{
		std::vector<float> inputKeyframes; //time
	};
	struct AnimationOutputValue
	{
		std::vector<float> outputValues;
	};

	struct AnimationSampler
	{
		AnimationInterpolationMode interpolationMode = AnimationInterpolationMode::UNKNOWN;
		int inputIndex = -1;
		int outputIndex = -1;
	};



	struct AnimationChannel
	{
		int targetNodeIndex;
		AnimationPathType pathType;
		int samplerIndex; //index to sampler list
	};


	struct AnimationData
	{ 
		/** STAND ALONE **/
		//Master struct to hold all animation data
		std::vector<AnimationTimeInput> inputs;
		std::vector<AnimationOutputValue> outputs;

	};
	struct Animation
	{
		std::vector<AnimationChannel> animationChannel;
		std::vector<AnimationSampler> animationSamplers;

		float maxTime = 0.0f;
	};

	enum class IndexFormat
	{
		UINT8,
		UINT16,
		UINT32
	};





	struct MaterialPBRMetallicRoughnessTexture
	{ //PBR GlossSpecular workflow
		GLuint baseColourTexture;
		GLuint metallicRoughnessTexture;

		//optional
		//not related to pbr
		//This textures are arbitarily available like it could have one or two or none
		GLuint normalTexture;
		GLuint occulsionTexture;//1 
		GLuint emmissiveTexture;


	};


	struct MaterialPBRMetallicRoughnessConstValues // send to shader as uniform buffer
	{
		//Gonna be send to shader so need to pad
		//PBR metallicRoughness workflow
		glm::vec4 baseColorFactor{};
		glm::vec3 emmissiveFactor{};//optional

		float metallicFactor = 0.0f;
		float roughnessFactor = 0.0f;

		//bool pbrTexturesPresent = false;
		uint32_t baseColorTexturePresent = false;
		uint32_t metallicRoughnessTexturePresent = false;

		uint32_t normalTexturePresent = false;
		uint32_t occulsionTexturePresent = false;
		uint32_t emmissiveTexturePresent = false;

	};



	struct Material
	{
		MaterialPBRMetallicRoughnessConstValues materialPBRMetallicRoughnessConstValues;
		MaterialPBRMetallicRoughnessTexture materialPBRMetallicRoughnessTexture;

	};




	enum class VertexPackingFormat
	{
		INTERLEAVED,
		NON_INTERLEAVED,
		UNKNOWN
	};

	enum class VertexAttributes
	{
		//interleaved only 
		POSITION,
		NORMAL,
		TEXCOORD_0,
		WEIGHT,
		JOINT,
		UNKNOWN

	};





	struct Primitive
	{
		//only interleaved 

		size_t vertexOffsetInBuffer;
		size_t vertexCount;
		size_t stride = 0;

		size_t indexOffsetInBuffer;
		size_t indexCount;//shouldnt it also be an offset?
		bool isIndexed; // we currently dont have index buffer
		IndexFormat indexFormat;

		int materialIndex;
		
		bool isSkinned = false;
	};


	struct Mesh
	{
		std::vector<Primitive> primitives;
	};


	struct Transformation
	{
		glm::mat4 localTransformMatrix = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		glm::mat4 worldTransformMatrix = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);//used by rendersystem

		glm::vec3 position{ 0.0f,0.0f,0.0f };
		glm::vec3 scale{ 1.0f,1.0f,1.0f };
		glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	};



	struct EngineNode
	{
		Transformation transformation;

		std::vector<int> children;//indices only

		int meshIndex = -1;//index to mesh list
		bool isSkinned = false;
	};


	struct Model
	{
		size_t rootNodeIndex = 0;

		std::vector<EngineNode> nodes;
		std::vector<Material> materials;
		std::vector<Mesh> meshes;

		std::unordered_map<std::string, Animation> animations;

		struct AnimationState
		{
			float totalDuration = 0.0f; // Max keyframe time across all channels
			float currentTime = 0.0f;   // Current time in the animation (0.0 to totalDuration)

		};


		AnimationData animationData;

		std::map<std::string, AnimationState> playingAnimations;


		/*Skinning*/
		bool hasSkin = false;
		bool currentlyAnimated = false;// for not running bone system everytime

		std::vector<glm::mat4> inverseBindMatrices;
		std::vector<int> jointIndices; // 

		std::vector<glm::mat4> jointMatrices; // should be pre-reserved size and indexable


	};
}