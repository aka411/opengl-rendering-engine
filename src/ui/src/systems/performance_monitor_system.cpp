
#include "../../../../include/ui/include/utils/geometry_generator.h"
#include "../../../../include/ui/include/systems/performance_monitor_system.h"
#include "../../../../include/ui/include/core/ui_component.h"
#include "../../../../include/ui/include/core/ui_core_system.h"



PerformanceMonitorSystem::PerformanceMonitorSystem(UI::UICoreSystem& uiCoreSystem) :m_uiCoreSystem(uiCoreSystem), m_ecsEngine(uiCoreSystem.getECSEngine())
{

	m_ecsEngine.registerComponent<UI::UIGraphComponent>();



}

void PerformanceMonitorSystem::setUp(UI::UIElement& fpsGraphElement)
{
	
	m_fpsGraphElement = &fpsGraphElement;


}

void PerformanceMonitorSystem::UpdateFPSMeter(const float deltaTime)
{

	assert(m_fpsGraphElement != nullptr);

	ECS::EntityChunkView entityChunkView = m_ecsEngine.getEntityChunkView(m_fpsGraphElement->getEntityId());

	UI::UIGraphComponent* uiGraphComponentPtr = entityChunkView.getComponent<UI::UIGraphComponent>();
	UI::UIRenderMeshComponent* uiRenderMeshComponent = entityChunkView.getComponent<UI::UIRenderMeshComponent>();
	UI::UIRectDimensionsComponent* uiRectDimensionsComponentPtr = entityChunkView.getComponent<UI::UIRectDimensionsComponent>();
	


	uiGraphComponentPtr->store(1.0f/(deltaTime+0.0000000001));

	//TODO : need more optimization
	//generate mesh
	const float maxValue = uiGraphComponentPtr->maxValue;
	std::vector<float> data = uiGraphComponentPtr->getData();

	float offset = 0;//x axis

	std::vector<std::byte> resultMesh;

	resultMesh.reserve(uiGraphComponentPtr->buffer.size() * (4 * 2) * sizeof(float));

	uiRenderMeshComponent->vertexCount = 0;
	for (float dataPoint : data)
	{
		//100 px = 500 fps
		//if (dataPoint < 0.0001) continue;
		float scaledHeight = (dataPoint/500.0f ) * 100.0f;


		//NOTE : The scaledHeight is negated cause the 
		//geometry generates the rectangles with Y axis up but our UI coordinate system has Y axis down
		GeometryGenerator::MeshData mesh = GeometryGenerator::getColouredRectangleWithOffset(6,-scaledHeight, { offset ,100,0}, uiGraphComponentPtr->color);
		offset += 7;

		if (uiRectDimensionsComponentPtr->width < offset) break;
		uiRenderMeshComponent->vertexCount += mesh.numOfVertex;

		resultMesh.insert(
			resultMesh.end(),
			mesh.data.begin(),
			mesh.data.end()
		);
	}
	const auto vertexFormat = uiRenderMeshComponent->vertexFormat;
	const auto bufferType = uiRenderMeshComponent->bufferType;

	

	size_t bufferOffset = m_uiCoreSystem.getUIVertexBufferManagementSystem().uploadVertexData(vertexFormat, bufferType, resultMesh.data(), resultMesh.size());


	//uiRenderMeshComponent->vertexCount =;

	uiRenderMeshComponent->vertexBufferOffset = bufferOffset;
	uiRenderMeshComponent->isDirty = false;



}

void PerformanceMonitorSystem::UpdateMemoryUsage(const size_t UsedBytes)
{



}
