#pragma once 
#include "gpu_texture_manager.h"
#include "buffer_management_system.h"
#include "tiny_gltf.h"

#include "engine_data_structures.h"
#include "vertex_attribute_repacker.h"

namespace Engine
{




	class GLTFFlatParser
	{
	private:


		BufferManagementSystem& m_bufferManagementSystem;
		GPUTextureManager& m_gpuTextureManager;

		

		std::vector<GLuint> m_imageList;
		std::vector<GLuint> m_textureList;
		std::vector<Material> m_materialList;




		Transformation getTransformation(tinygltf::Node node);

		std::vector<GLuint> extractTextures(tinygltf::Model& tinygltfModel);
		std::vector<GLuint> extractImages(tinygltf::Model& tinygltfModel);
		std::vector<Material> extractMaterial(tinygltf::Model& tinygltfModel);

		std::vector<Mesh> getMeshList(tinygltf::Model& tinygltfModel);
		std::vector<EngineNode> getNodeList(tinygltf::Model& tinygltfModel);



		bool checkIfInterleaved(VertexAttributeGltfLocationMap& vertexAttributeGltfLocationMap);

		VertexAttributeGltfLocationMap getVertexAttributeMap(tinygltf::Primitive& primitive, tinygltf::Model& tinygltfModel);
		std::vector<Primitive> getPrimitives(tinygltf::Model& tinygltfModel, tinygltf::Mesh& gltfMesh);





		



	public:
		GLTFFlatParser(BufferManagementSystem& bufferManagementSystem, GPUTextureManager& gpuTextureManager);
		
		~GLTFFlatParser() = default;


		Model parse(tinygltf::Model& tinygltfModel);
	};

}