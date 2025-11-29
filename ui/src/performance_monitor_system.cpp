
#include "../include/performance_monitor_system.h"
#include "../include/builder/ui_builder.h"
#include "../include/geometry_generator.h"

PerformanceMonitorSystem::PerformanceMonitorSystem(TheEngine::ECS::ECSEngine* ecsEngine) : m_ecsEngine(ecsEngine)
{





}

void PerformanceMonitorSystem::setUp()
{
	m_ecsEngine->registerComponent<UI::UIGraphComponent>();

	m_fpsUIEntity = UI::UIBuilder::createUIPanel()
		.withColour({ 0.5f,0.3f,0.2f,0.7f })
		.withRectDimensions(400, 400)
		.withPosition({ 50,300,0 })
		.build();

	UI::UIGraphComponent uiGraphComponent;

	m_ecsEngine->addComponentToEntity(m_fpsUIEntity, uiGraphComponent);
	m_ecsEngine->processBufferedCommands();
}

void PerformanceMonitorSystem::UpdateFPSMeter(const float deltaTime)
{

	TheEngine::ECS::EntityChunkView entityChunkView = m_ecsEngine->getEntityChunkView(m_fpsUIEntity);

	UI::UIGraphComponent* uiGraphComponentPtr = entityChunkView.getComponent<UI::UIGraphComponent>();
	UI::UIRenderMeshComponent* uiRenderMeshComponent = entityChunkView.getComponent<UI::UIRenderMeshComponent>();

	uiGraphComponentPtr->store(1.0f/deltaTime);

	//generate mesh
	const float maxValue = uiGraphComponentPtr->maxValue;
	std::vector<float> data = uiGraphComponentPtr->getData();
	float offset = 0;
	std::vector<std::byte> resultMesh;
	resultMesh.reserve(uiGraphComponentPtr->buffer.size() * 6 * 3 * sizeof(float));
	uiRenderMeshComponent->vertexCount = 0;
	for (float dataPoint : data)
	{
		//if (dataPoint < 0.0001) continue;
		float scaledHeight = (dataPoint / (maxValue > 0 ? maxValue : 1.0f)) * 50.0f;
		GeometryGenerator::MeshData mesh = GeometryGenerator::getRectangleWithOffset(5,-scaledHeight, { offset ,0,0});
		offset += 6;
		uiRenderMeshComponent->vertexCount += mesh.numOfVertex;

		resultMesh.insert(
			resultMesh.end(),
			mesh.data.begin(),
			mesh.data.end()
		);
	}
	uiRenderMeshComponent->data = std::move(resultMesh);
	uiRenderMeshComponent->isDirty = true;

}

void PerformanceMonitorSystem::UpdateMemoryUsage(const size_t UsedBytes)
{



}
