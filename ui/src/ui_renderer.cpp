#include <glm/ext/vector_float3.hpp>
#include <vector>
#include "../../glad/glad.h"
#include <string>
#include "../include/ui_renderer.h"
#include <iostream>
#include "../include/ui_shader_code.h"
#include "../include/hierarchy_builder.h"
#include "../include/builder/ui_builder.h"
#include "../include/ui_transformation_system.h"


namespace UI
{

	GLuint UIRenderer::compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode)
	{




		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertShaderSource = vertShaderCode.c_str();
		const char* fragShaderSource = fragShaderCode.c_str();

		glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
		glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);



		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Check for fragment shader compilation errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}



		GLuint shaderProgram;

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		//cleanup
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);


		return shaderProgram;
	}











	UIRenderer::UIRenderer() : m_uiECSEngine(), m_uiMeshGenerationSystem(&m_uiECSEngine), m_uiVertexBufferSystem(), m_vertexBufferUploadManager(m_uiECSEngine, m_uiVertexBufferSystem), m_performanceGraphSystem(&m_uiECSEngine)
	{


		m_uiECSEngine.registerComponent<UI::UIRenderMeshComponent>();
		m_uiECSEngine.registerComponent<UI::UIMaterialComponent>();
		m_uiECSEngine.registerComponent<UI::UIRectDimensionsComponent>();
		m_uiECSEngine.registerComponent<UI::UIRectBoundsComponent>();
		m_uiECSEngine.registerComponent<UI::UITransformComponent>();
		m_uiECSEngine.registerComponent<UI::UITextComponent>();
		m_uiECSEngine.registerComponent<UI::UITextMeshComponent>();
		m_uiECSEngine.registerComponent<UI::UIRootComponent>();
		m_uiECSEngine.registerComponent<UI::UIChildrenComponent>();

		HierarchyBuilder::setECSEngine(m_uiECSEngine);
	    UI:UIBuilder::setECSEngine(m_uiECSEngine);

		m_uiShaderProgram = compileShader(normalUIShaderVertexCode, normalUIShaderFragmentCode);

		glCreateVertexArrays(1, &m_uiNormalVAO);
		glBindVertexArray(m_uiNormalVAO);


		glVertexAttribFormat(0,3, GL_FLOAT, false, 0);
		glVertexAttribBinding(0, 0);
		glEnableVertexAttribArray(0);





		m_performanceGraphSystem.setUp();

	}



	void UIRenderer::setPhysicalViewport(int width, int height)
	{


	}
	void UIRenderer::setVirtualViewport(int width, int height)
	{ 
	
	
	}

	void UIRenderer::runCustomCode()
	{

		m_uiCamera.setOrthographicProjection(0, 700, 700, 0, 10, -100);



		/**
		UIEntity uiEntity = UI::UIBuilder::createUIPanel()
			.withColour({ 1.0f,0.3f,0.5f,1.0f })
			.withRectDimensions(10, 10)
			.withPosition({ 350,350,-100 })
			.build();*/
			
		m_uiMeshGenerationSystem.generateUIMeshes();
		m_vertexBufferUploadManager.uploadAllVertexBuffers();

		UITransformationSystem::setECSEngine(m_uiECSEngine);

		UITransformationSystem::updateLocalTransformation();


	}

	void UIRenderer::updateFPS(const float deltaTime)
	{
		m_performanceGraphSystem.UpdateFPSMeter(deltaTime);
		m_vertexBufferUploadManager.uploadAllVertexBuffers();
	}

	void UIRenderer::renderUI()
	{


		glDisable(GL_DEPTH_TEST);


		glClearColor(0.4, 0.4, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_uiShaderProgram);
		
		glBindVertexArray(m_uiNormalVAO);


		GLuint projectionLoc = glGetUniformLocation(m_uiShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &m_uiCamera.getProjectionMatrix()[0][0]);

	


		TheEngine::ECS::Query query = m_uiECSEngine.getQuery<UIRenderMeshComponent,UITransformComponent>();

		for (auto& chunk : query.getChunkArrayViews())
		{
			const size_t count = chunk.getCount();

			for (size_t i = 0; i < count; ++i)
			{
				UIRenderMeshComponent* uiRenderMeshComponentPtr = chunk.getComponentArray<UIRenderMeshComponent>();
				UITransformComponent* uiTransformComponentPtr = chunk.getComponentArray<UITransformComponent>();

				GLuint modelLoc = glGetUniformLocation(m_uiShaderProgram, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &uiTransformComponentPtr->localTransfrom[0][0]);

				//change

				GPUBufferInfo vertexBufferInfo = m_uiVertexBufferSystem.getBufferInfoForVertexFormat(UIVertexFormat::NORMAL_UI_VERTEX_FORMAT);
				
				glBindVertexBuffer(0, vertexBufferInfo.bufferHandle,0, 3 * 4);


				glDrawArrays(GL_TRIANGLES, uiRenderMeshComponentPtr[i].vertexBufferOffset/12, uiRenderMeshComponentPtr[i].vertexCount);

				

			}


		}





		//set camera uniforms
		//use binding point 0 for camera uniform buffer










	}








}