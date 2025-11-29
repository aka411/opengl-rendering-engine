#pragma once
#include "ui_vertex_buffer_system.h"
#include "../../include/camera.h"

#include <array>
#include "../../external/the-engine/the-engine/ecs/include/ecs_engine.h"
#include "ui_mesh_generation_system.h"
#include "vertex_buffer_upload_manager.h"
#include "performance_monitor_system.h"

namespace UI
{
	class UIRenderer
	{
	private:
		//gets ui ecs system
		TheEngine::ECS::ECSEngine m_uiECSEngine;
		struct ViewPort
		{
			int width = 0;
			int height = 0;
			float aspectRatio = 1.0f; // width / height

		};

		ViewPort m_physicalViewport;
		ViewPort m_virtualViewport;


		GLuint m_uiShaderProgram;
		//GLuint m_uiTextShaderProgram;

		GLuint m_uiNormalVAO;

		GLuint compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode);

		

		Engine::Camera m_uiCamera;




		UIMeshGenerationSystem m_uiMeshGenerationSystem;
		UIVertexBufferSystem m_uiVertexBufferSystem;
		VertexBufferUploadManager m_vertexBufferUploadManager;


		PerformanceMonitorSystem m_performanceGraphSystem;

	public:
	
		UIRenderer();

		void setPhysicalViewport(int width, int height);
		void setVirtualViewport(int width, int height);

		void updateFPS(const float deltaTime);
		void runCustomCode();

		void renderUI();
	};


}