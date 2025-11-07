#pragma once 
#include <vector>
#include <glm/ext/quaternion_float.hpp>
#include "../glad/glad.h"

#include "gpu_texture_manager.h"
#include "buffer_management_system.h"

#include "tiny_gltf.h"

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
	{  //Master struct to hold all animation data
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
		UNSIGNED_SHORT,
		UNSIGNED_INT
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
		glm::vec4 emmissiveFactor{};//optional

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

	/*
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;

	};*/



	enum class VertexPackingFormat
	{
		INTERLEAVED,
		NON_INTERLEAVED
	};

	enum class VertexAttributes
	{
		//interleaved only 
		POSITION,
		NORMAL,
		TEXCOORD,
		WEIGHT,
		JOINT

	};



	struct VertexInfo
	{
		VertexPackingFormat vertexPackingFormat;

	};


	struct Primitive
	{
		VertexInfo vertexInfo;

		//size_t offsetInBuffer;
		size_t vertexCount;
		size_t indexCount;//shouldnt it also be an offset?


		bool isIndexed; // we currently dont have index buffer
		IndexFormat indexFormat;
		size_t indexOffsetInBuffer;
		//size_t materialIndex;

		size_t positionOffsetInBuffer;
		size_t postionStride = 0;

		size_t normalOffsetInBuffer;
		size_t normalStride = 0;

		size_t texCoordOffsetInBuffer;
		size_t texCoordStride = 0;
		bool texCoordPresent = false;


		int materialIndex;
		//Material material; //for now each primitive has its own material later we can optimize it


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


	class GLTFFlatParser
	{
	private:


		BufferManagementSystem& m_bufferManagementSystem;
		GPUTextureManager& m_gpuTextureManager;

		tinygltf::Model* m_model = nullptr;

		std::vector<GLuint> m_imageList;
		std::vector<GLuint> m_textureList;

		std::vector<Material> m_materialList;


		//std::vector<AnimationSampler> m_animationSamplers;
		std::unordered_map<std::string, Animation> m_animationMap;

		AnimationData m_animationData;


		size_t m_globalVertexBufferGlobalOffset = 0;



		glm::mat4 convertDoubleToFloatMatrix4(const std::vector<double>& tinygltfMatrix4Data);




		glm::vec4 convertDoubleToFloatVec4(const std::vector<double>& tinygltfVec4Data);

		glm::vec3 convertDoubleToFloatVec3(const std::vector<double>& tinygltfVec3Data);
		glm::quat convertDoubleToFloatQuat(const std::vector<double>& tinygltfQuatData);

		Transformation getTransformation(tinygltf::Node node);

		std::vector<GLuint> parseTextures(tinygltf::Model& tinygltfModel);
		std::vector<GLuint> parseImages(tinygltf::Model& tinygltfModel);
		std::vector<Material> extractMaterial(tinygltf::Model& tinygltfModel);
		std::vector<Primitive> getPrimitives(tinygltf::Model& tinygltfModel, tinygltf::Mesh& mesh, size_t globalVertexBufferGlobalOffset);

		std::vector<Mesh> getMeshList(tinygltf::Model&  tinygltfModel);


		std::vector<EngineNode> getNodeList(tinygltf::Model& tinygltfModel);


		AnimationPathType getAnimationType(std::string& animationPath);

		AnimationInterpolationMode getAnimationInterpolationMode(std::string& tinygltfAnimationInterpolationMode);
		void getAnimation(tinygltf::Model& tinygltfModel);
		//void extractAnimationSamplers(tinygltf::Model& tinygltfModel);



	public:
		GLTFFlatParser::GLTFFlatParser(BufferManagementSystem& bufferManagementSystem, GPUTextureManager& gpuTextureManager);
		
		~GLTFFlatParser() = default;


		Model parse(tinygltf::Model& tinygltfModel);
	};

}