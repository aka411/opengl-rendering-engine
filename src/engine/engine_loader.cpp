#include "../../include/engine/engine_loader.h"




EngineLoader::EngineLoader(TheEngine::ECS::ECSEngine& ecsEngine, GPUTextureManager& gpuTextureManager, GPUMaterialSystem& gpuMaterialSystem, WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem)
	: m_ecsEngine(ecsEngine), m_gpuTextureManager(gpuTextureManager), m_gpuMaterialSystem(gpuMaterialSystem), m_worldVertexBufferManagementSystem(worldVertexBufferManagementSystem)
{




}

TheEngine::ECS::EntityId EngineLoader::createRootEntity(const std::string& pathToModel)
{
	//get gltf model

	tinygltf::Model gltfModel = loadGLTFModel(pathToModel);


	EngineIntermediateModel  engineIntermediateModel = m_gltfIntermediateMapper.convertGltfToEngineIntermediateModel(gltfModel);


	//upload images and get id 
	std::vector<uint64_t> localImageIndexToImageHandle;
	localImageIndexToImageHandle.reserve(engineIntermediateModel.intermediateImage.size());
	for (auto& image : engineIntermediateModel.intermediateImage)
	{
		TextureCreateInfo textureCreateInfo;
		textureCreateInfo.width = image.width;
		textureCreateInfo.height = image.height;
		textureCreateInfo.textureSourceComponentType = image.textureSourceComponentType;
		textureCreateInfo.textureSourcePixelFormat = image.textureSourcePixelFormat;
		textureCreateInfo.internalFormat = image.internalFormat;


		textureCreateInfo.type =TextureType::TEXTURE_2D ;

		//textureCreateInfo.samplerSetting =;

		textureCreateInfo.data = image.data.data();

		const uint64_t imageHandle = m_gpuTextureManager.createNewTexture(textureCreateInfo).resisdentHandle;
		localImageIndexToImageHandle.push_back(imageHandle);
	}





	std::vector<uint64_t> localTextureToEngineTexture;
	localTextureToEngineTexture.reserve(engineIntermediateModel.intermediateTextures.size());

	for (auto& texture : engineIntermediateModel.intermediateTextures)
	{ 
		
		localTextureToEngineTexture.push_back(localImageIndexToImageHandle[texture.imageIndex]);
		
	}

	
	std::vector<MaterialId> localToEnginePBRMaterial;
	localToEnginePBRMaterial.reserve(engineIntermediateModel.intermdediatePBRMetallicRoughnessMaterials.size());


	auto getCorrectTextureIndex = [](int index, const std::vector<uint64_t>& textures)->uint64_t
		{
			if (index < 0) { return 0; }
			return textures[index];
		};

	for (auto& material : engineIntermediateModel.intermdediatePBRMetallicRoughnessMaterials)
	{
		PBRMetallicRoughnessMaterial pbrMetallicRoughnessMaterial;

		pbrMetallicRoughnessMaterial.albedoTextureHandle = getCorrectTextureIndex(material.albedoTextureIndex, localTextureToEngineTexture);
		pbrMetallicRoughnessMaterial.metallicRoughnessTextureHandle = getCorrectTextureIndex(material.metallicRoughnessTextureIndex, localTextureToEngineTexture);
		pbrMetallicRoughnessMaterial.normalTextureHandle = getCorrectTextureIndex(material.normalTextureIndex, localTextureToEngineTexture);
		pbrMetallicRoughnessMaterial.emissiveTextureHandle = getCorrectTextureIndex(material.emissiveTextureIndex, localTextureToEngineTexture);
		pbrMetallicRoughnessMaterial.occlusionTextureHandle = getCorrectTextureIndex(material.occlusionTextureIndex, localTextureToEngineTexture);


		pbrMetallicRoughnessMaterial.baseColorFactor = material.baseColorFactor;
		pbrMetallicRoughnessMaterial.emissiveFactor = material.emissiveFactor;
		pbrMetallicRoughnessMaterial.metallicFactor = material.metallicFactor;
		pbrMetallicRoughnessMaterial.roughnessFactor = material.roughnessFactor;

		pbrMetallicRoughnessMaterial.configMask = material.configMask;


		MaterialId materialId = m_gpuMaterialSystem.uploadMaterial(MaterialType::PBR_METALLIC_ROUGHNESS, reinterpret_cast<std::byte*>(&pbrMetallicRoughnessMaterial), sizeof(pbrMetallicRoughnessMaterial));

		localToEnginePBRMaterial.push_back(materialId);
	
	
	}





	/*-------VERTEX DATA-------*/
	std::vector<size_t> engineVertexoffsets;
	engineVertexoffsets.reserve(engineIntermediateModel.vertexDatas.size());
	for (auto& vertex : engineIntermediateModel.vertexDatas)
	{
		size_t engineVertexOffset = m_worldVertexBufferManagementSystem.uploadVertexData(vertex.vertexFormat, vertex.data.data(), vertex.data.size());
		engineVertexoffsets.push_back(engineVertexOffset);

	}


	std::vector<size_t> engineIndexoffsets;
	engineIndexoffsets.reserve(engineIntermediateModel.indiceDatas.size());
	for (auto& index : engineIntermediateModel.indiceDatas)
	{
		size_t engineIndexOffset = m_worldVertexBufferManagementSystem.uploadIndexData(index.indexType, index.data.data(), index.data.size());
		engineIndexoffsets.push_back(engineIndexOffset);

	}






	struct EngineMesh
	{
		EngineRenderComponent engineRenderComponent;

		bool isfat = false;
		EngineFatRenderComponent  engineFatRenderComponent;
	};







	std::vector<EngineMesh> m_engineMeshs;
	m_engineMeshs.reserve(engineIntermediateModel.intermediateMeshs.size());

	for (const auto& intermdeiateMesh : engineIntermediateModel.intermediateMeshs)
	{
		EngineMesh engineMesh;

		engineMesh.isfat = (intermdeiateMesh.intermediatePrimitives.size() > 1)? true : false;

		

		if (engineMesh.isfat)
		{
			engineMesh.engineFatRenderComponent.engineRenderComponents.reserve(intermdeiateMesh.intermediatePrimitives.size());
			for (auto& primitive : intermdeiateMesh.intermediatePrimitives)
			{
				EngineRenderComponent engineRenderComponent;

				const size_t vertexOffset = engineVertexoffsets[primitive.vertexDataId];
				const size_t vertexCount = engineIntermediateModel.vertexDatas[primitive.vertexDataId].vertexCount;
				const VertexFormat vertexFormat = engineIntermediateModel.vertexDatas[primitive.vertexDataId].vertexFormat;
				

				engineRenderComponent.vertexOffset = vertexOffset;
				engineRenderComponent.vertexCount = vertexCount;
				engineRenderComponent.vertexFormat = vertexFormat;


				engineRenderComponent.materialId = localToEnginePBRMaterial[primitive.materialId];

				if (primitive.isIndexed)
				{
					engineRenderComponent.isIndexed = true;

					const IndexType indexType = engineIntermediateModel.indiceDatas[primitive.indexId].indexType;
					const size_t indexCount = engineIntermediateModel.indiceDatas[primitive.indexId].indexCount;
					const size_t indexOffset = engineIndexoffsets[primitive.indexId];

					engineRenderComponent.indexType = indexType;
					engineRenderComponent.indexCount = indexCount;
					engineRenderComponent.indexOffset = indexOffset;

				}


				
				
				
				


				engineMesh.engineFatRenderComponent.engineRenderComponents.push_back(engineRenderComponent);
			}

			
		}
		else
		{

			assert(intermdeiateMesh.intermediatePrimitives.size() != 0);

			const IntermediatePrimitive& primitive = intermdeiateMesh.intermediatePrimitives[0];
			EngineRenderComponent engineRenderComponent;

			const size_t vertexOffset = engineVertexoffsets[primitive.vertexDataId];
			const size_t vertexCount = engineIntermediateModel.vertexDatas[primitive.vertexDataId].vertexCount;
			const VertexFormat vertexFormat = engineIntermediateModel.vertexDatas[primitive.vertexDataId].vertexFormat;


			engineRenderComponent.vertexOffset = vertexOffset;
			engineRenderComponent.vertexCount = vertexCount;
			engineRenderComponent.vertexFormat = vertexFormat;


			engineRenderComponent.materialId = localToEnginePBRMaterial[primitive.materialId];

			if (primitive.isIndexed)
			{
				engineRenderComponent.isIndexed = true;

				const IndexType indexType = engineIntermediateModel.indiceDatas[primitive.indexId].indexType;
				const size_t indexCount = engineIntermediateModel.indiceDatas[primitive.indexId].indexCount;
				const size_t indexOffset = engineIndexoffsets[primitive.indexId];

				engineRenderComponent.indexType = indexType;
				engineRenderComponent.indexCount = indexCount;
				engineRenderComponent.indexOffset = indexOffset;

			}


			engineMesh.engineRenderComponent = engineRenderComponent;



		}

		m_engineMeshs.push_back(engineMesh);

	}








	std::vector<TheEngine::ECS::EntityId> engineEntityIds;
	engineEntityIds.reserve(engineIntermediateModel.engineIntermediateNode.size());

	for (auto& node : engineIntermediateModel.engineIntermediateNode)
	{

		engineEntityIds.push_back(m_ecsEngine.createEntity());

		   
	}




	//we have added render component , transformation component , children component
	for (size_t i = 0 ; i <  engineIntermediateModel.engineIntermediateNode.size(); ++i)
	{
	    EngineIntermediateNode node = engineIntermediateModel.engineIntermediateNode[i];
		const TheEngine::ECS::EntityId entityId = engineEntityIds[i];

		m_ecsEngine.addComponentToEntity<EngineTransformationComponent>(entityId, node.transformation);//transformation



		EngineMesh& engineMesh = m_engineMeshs[node.meshIndex];

		if (engineMesh.isfat)
		{
			m_ecsEngine.addComponentToEntity<EngineFatRenderComponent>(entityId, engineMesh.engineFatRenderComponent);

		}
		else
		{
			m_ecsEngine.addComponentToEntity<EngineRenderComponent>(entityId, engineMesh.engineRenderComponent);
		}
		

		if (node.children.size() > 0)
		{
			EngineChildrenComponent engineChildrenComponent;
			engineChildrenComponent.childrenEntities.reserve(node.children.size());
			for (auto& childIndex : node.children)
			{
				engineChildrenComponent.childrenEntities.push_back(engineEntityIds[childIndex]);
			}

			m_ecsEngine.addComponentToEntity<EngineChildrenComponent>(entityId,engineChildrenComponent);
		}


	}





	TheEngine::ECS::EntityId rootEntityId = engineEntityIds[engineIntermediateModel.rootNodeIndex];
	RootEntityComponent rootEntityComponent{};
	m_ecsEngine.addComponentToEntity<RootEntityComponent>(rootEntityId, rootEntityComponent);

	m_ecsEngine.processBufferedCommands();

	return rootEntityId;
}


tinygltf::Model EngineLoader::loadGLTFModel(const std::string& pathToModel)
{

	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	



	bool ret = false;

	if (pathToModel.substr(pathToModel.length() - 4) == "gltf")
	{
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, pathToModel);
	}
	else
	{
		ret = loader.LoadBinaryFromFile(&model, &err, &warn, pathToModel); // for binary glTF(.glb)
	}

	if (!warn.empty())
	{
		printf("Warn: %s\n", warn.c_str());
	}

	if (!err.empty())
	{
		printf("Err: %s\n", err.c_str());
		assert(0);
	}

	if (!ret)
	{
		printf("Failed to parse glTF\n");
		assert(0);
	}

	
	return model;

}