#include "../../include/rendering-system/render_system.h"
#include <unordered_map>
#include "../../include/low-level/vertex_format_helper.h"

namespace Engine
{

	RenderSystem::RenderSystem(ECS::ECSEngine& ecsEngine, WorldVertexBufferManagementSystem& worldVertexBufferManagementSystem, GPUMaterialSystem& gpuMaterialSystem, UI::UICoreSystem& uiCoreSystem, AnimationSystem& animationSystem, GPUBufferManager& gpuBufferManager)
		:
		m_vertexFormatManager(),
		m_ecsEngine(ecsEngine),
		m_worldVertexBufferManagementSystem(worldVertexBufferManagementSystem),
		m_uiRenderer(uiCoreSystem),
		m_animationSystem(animationSystem),
		m_renderCommandBufferManager(gpuBufferManager),
		m_objectDataBufferManager(gpuBufferManager),
		m_gpuBufferManager(gpuBufferManager),
		m_worldRenderer(m_vertexFormatManager, worldVertexBufferManagementSystem, gpuMaterialSystem, animationSystem, m_renderCommandBufferManager, m_objectDataBufferManager)

	{



	}



	void RenderSystem::render(Engine::Camera& camera)
	{


		
		m_worldRenderer.startFrame(camera);
	

		ECS::Query querySingleRenderableComponent = m_ecsEngine.getQuery<EngineTransformationComponent, EngineRenderComponent>();

		ECS::Query queryFatRenderableComponent = m_ecsEngine.getQuery<EngineTransformationComponent, EngineFatRenderComponent>();

		//indexed -> vertexformat
		//non-indexed -> vertexformat

		//copy this to buffer

		std::vector<ObjectData> objectDatas;

		//




		struct BatchArrayCommand
		{
			std::vector<IndirectArrayRenderCommand> indirectArrayRenderCommand;
		};

		
		struct BatchIndexedCommand
		{
			std::vector<IndirectIndexedRenderCommand> indirectIndexedRenderCommand;
		};

		std::unordered_map<VertexFormat, BatchArrayCommand> batchArrayCommands;

		std::unordered_map<VertexFormat, std::unordered_map<IndexType,BatchIndexedCommand>> batchIndexedCommandsPerIndexTypes;




		//i need to fill this first then upload and replace with offsets





		for (auto& chunkArrayView : querySingleRenderableComponent.getChunkArrayViews())
		{
			const size_t count = chunkArrayView.getCount();

			for (size_t i = 0; i < count; ++i)
			{
				EngineTransformationComponent* engineTransformationComponentArray = chunkArrayView.getComponentArray<EngineTransformationComponent>();
				EngineRenderComponent* engineRenderComponentArray = chunkArrayView.getComponentArray<EngineRenderComponent>();
				BoneJointMatrixIdComponent* BoneJointMatrixIdComponentArray = chunkArrayView.getComponentArray<BoneJointMatrixIdComponent>();


				EngineTransformationComponent& engineTransformationComponent = engineTransformationComponentArray[i];
				EngineRenderComponent& engineRenderComponent = engineRenderComponentArray[i];
				BoneJointMatrixId boneJointMatrixId = 0;
				if (BoneJointMatrixIdComponentArray != nullptr)
				{
					boneJointMatrixId = BoneJointMatrixIdComponentArray[i].JointIdInSSBO;
				}

				ObjectData objectData;
				objectData.modelMatrix = engineTransformationComponent.worldTransformMatrix;
				objectData.materialId = engineRenderComponent.materialId;

				objectData.boneId = boneJointMatrixId;

				objectDatas.push_back(objectData);

				if (engineRenderComponent.isIndexed)
				{
					IndirectIndexedRenderCommand indirectIndexedRenderCommand;

					indirectIndexedRenderCommand.instanceCount = 1;
					indirectIndexedRenderCommand.firstIndex = engineRenderComponent.indexOffset/ VertexFormatHelper::getSizeOfIndexTypeInBytes(engineRenderComponent.indexType);
					indirectIndexedRenderCommand.count = engineRenderComponent.indexCount;
					indirectIndexedRenderCommand.baseVertex = engineRenderComponent.vertexOffset/ VertexFormatHelper::getSizeOfVertexForFormatInBytes(engineRenderComponent.vertexFormat);
					indirectIndexedRenderCommand.baseInstance = objectDatas.size() - 1;

			

					batchIndexedCommandsPerIndexTypes[engineRenderComponent.vertexFormat][engineRenderComponent.indexType]
						.indirectIndexedRenderCommand.push_back(indirectIndexedRenderCommand);



				}
				else
				{
					IndirectArrayRenderCommand indirectArrayRenderCommand;

					indirectArrayRenderCommand.instanceCount = 1;
					indirectArrayRenderCommand.count = engineRenderComponent.vertexCount;
					indirectArrayRenderCommand.firstVertex = engineRenderComponent.vertexOffset / VertexFormatHelper::getSizeOfVertexForFormatInBytes(engineRenderComponent.vertexFormat);
					indirectArrayRenderCommand.baseInstance = objectDatas.size() -1;

					batchArrayCommands[engineRenderComponent.vertexFormat]
						.indirectArrayRenderCommand.push_back(indirectArrayRenderCommand);

				}




			}


		}






		for (auto& chunkArrayView : queryFatRenderableComponent.getChunkArrayViews())
		{
			const size_t count = chunkArrayView.getCount();

			for (size_t i = 0; i < count; ++i)
			{
				EngineTransformationComponent* engineTransformationComponentArray = chunkArrayView.getComponentArray<EngineTransformationComponent>();
				EngineFatRenderComponent* engineFatRenderComponentArray = chunkArrayView.getComponentArray<EngineFatRenderComponent>();


				EngineTransformationComponent& engineTransformationComponent = engineTransformationComponentArray[i];
				EngineFatRenderComponent& engineFatRenderComponent = engineFatRenderComponentArray[i];

				BoneJointMatrixIdComponent* boneJointMatrixIdComponentArray = chunkArrayView.getComponentArray<BoneJointMatrixIdComponent>();


		
				BoneJointMatrixId boneJointMatrixId = 0;
				if (boneJointMatrixIdComponentArray != nullptr)
				{
					boneJointMatrixId = boneJointMatrixIdComponentArray[i].JointIdInSSBO;
				}

				for (auto& engineRenderComponent : engineFatRenderComponent.engineRenderComponents)
				{
					

			
					ObjectData objectData;
					objectData.modelMatrix = engineTransformationComponent.worldTransformMatrix;
					objectData.materialId = engineRenderComponent.materialId;

					objectData.boneId = boneJointMatrixId;

					objectDatas.push_back(objectData);

					if (engineRenderComponent.isIndexed)
					{
						IndirectIndexedRenderCommand indirectIndexedRenderCommand;

						indirectIndexedRenderCommand.instanceCount = 1;
						indirectIndexedRenderCommand.firstIndex = engineRenderComponent.indexOffset / VertexFormatHelper::getSizeOfIndexTypeInBytes(engineRenderComponent.indexType);;
						indirectIndexedRenderCommand.count = engineRenderComponent.indexCount;
						indirectIndexedRenderCommand.baseVertex = engineRenderComponent.vertexOffset / VertexFormatHelper::getSizeOfVertexForFormatInBytes(engineRenderComponent.vertexFormat);
						indirectIndexedRenderCommand.baseInstance = objectDatas.size() - 1;



						batchIndexedCommandsPerIndexTypes[engineRenderComponent.vertexFormat][engineRenderComponent.indexType]
							.indirectIndexedRenderCommand.push_back(indirectIndexedRenderCommand);



					}
					else
					{
						IndirectArrayRenderCommand indirectArrayRenderCommand;

						indirectArrayRenderCommand.instanceCount = 1;
						indirectArrayRenderCommand.count = engineRenderComponent.vertexCount;
						indirectArrayRenderCommand.firstVertex = engineRenderComponent.vertexOffset / VertexFormatHelper::getSizeOfVertexForFormatInBytes(engineRenderComponent.vertexFormat);
						indirectArrayRenderCommand.baseInstance = objectDatas.size() - 1;

						batchArrayCommands[engineRenderComponent.vertexFormat]
							.indirectArrayRenderCommand.push_back(indirectArrayRenderCommand);

					}




				}


			}
		}















		//UPLOAD DATA

		m_objectDataBufferManager.uploadDataForCurrentFrame(
			reinterpret_cast<std::byte*>(objectDatas.data()),
			sizeof(objectDatas[0]) * objectDatas.size()
		);




		for (auto& batchArrayCommand : batchArrayCommands)
		{
			const VertexFormat vertexFormat = batchArrayCommand.first;
			std::vector<IndirectArrayRenderCommand>& IndirectArrayRenderCommands = batchArrayCommand.second.indirectArrayRenderCommand;
			
			const size_t byteOffset = m_renderCommandBufferManager.uploadArrayCommandsForCurrentFrame(
				reinterpret_cast<std::byte*>(IndirectArrayRenderCommands.data()),
				sizeof(IndirectArrayRenderCommands[0]) * IndirectArrayRenderCommands.size()
				);

			const size_t count = IndirectArrayRenderCommands.size();

			//call draw

			m_worldRenderer.IndirectDrawArray(vertexFormat, byteOffset, count);


		}


		for (auto& batchIndexedCommandsPerIndexType : batchIndexedCommandsPerIndexTypes)
		{
			const VertexFormat vertexFormat = batchIndexedCommandsPerIndexType.first;
			std::unordered_map<IndexType, BatchIndexedCommand>& batchIndexedCommandToIndexType = batchIndexedCommandsPerIndexType.second;

			for (auto& batchIndexedCommand : batchIndexedCommandToIndexType)
			{
				const IndexType indexType = batchIndexedCommand.first;
				std::vector<IndirectIndexedRenderCommand>& IndirectIndexedRenderCommands = batchIndexedCommand.second.indirectIndexedRenderCommand;


				const size_t byteOffset = m_renderCommandBufferManager.uploadIndexedCommandsForCurrentFrame(
					reinterpret_cast<std::byte*>(IndirectIndexedRenderCommands.data()),
					sizeof(IndirectIndexedRenderCommands[0]) * IndirectIndexedRenderCommands.size()
				);

				const size_t count = IndirectIndexedRenderCommands.size();
				//draw

				m_worldRenderer.IndirectDrawIndexed(vertexFormat, indexType, byteOffset, count);


			}
		}
	



		m_worldRenderer.endFrame();

	}

	void RenderSystem::renderUI()
	{



		m_uiRenderer.renderUI();




	}
}