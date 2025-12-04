#include "../../../include/ui/include/ui_renderer.h"
#include "../../../glad/glad.h"
#include "../../../include/ui/include/core/ui_component.h"
#include "../../../include/components.h"



namespace UI
{


	UIRenderer::UIRenderer(UICoreSystem& uiCoreSystem) : m_uiCoreSystem(uiCoreSystem)
	{

		
		glCreateVertexArrays(1, &m_dummyVAO);
		//GODAMN BUG ,Need valid vao binded for vertx pulling to work


		m_uiCamera.setOrthographicProjection(0, 700, 700, 0, 10, -100);

	}


	void UIRenderer::renderUI()
	{

		/****PER FRAME STATE****/
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.4, 0.4, 0.5, 1.0);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(m_dummyVAO);//Even though we are using 
		//vertex pulling we need a dummy VAO binded for it to work
		



		/**PER FORMAT STATE***/

		GLuint uiShaderProgram = m_uiCoreSystem.getUIShaderManager().getShaderProgramForVertexFormat(UIVertexFormat::NORMAL_UI_VERTEX_FORMAT);


		glUseProgram(uiShaderProgram);



		GLuint projectionLoc = glGetUniformLocation(uiShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &m_uiCamera.getProjectionMatrix()[0][0]);

	
		TheEngine::ECS::ECSEngine& ecsEngine = m_uiCoreSystem.getECSEngine();

		TheEngine::ECS::Query query = ecsEngine.getQuery<UIRenderMeshComponent,EngineTransformationComponent>();


		UIVertexBufferManagementSystem& uiVertexBufferManagementSystem = m_uiCoreSystem.getUIVertexBufferManagementSystem();



		for (auto& chunk : query.getChunkArrayViews())
		{
			const size_t count = chunk.getCount();
			UIRenderMeshComponent* uiRenderMeshComponentPtr = chunk.getComponentArray<UIRenderMeshComponent>();
			EngineTransformationComponent* engineTransformationComponent = chunk.getComponentArray<EngineTransformationComponent>();

	
			for (size_t i = 0; i < count; ++i)
			{
			
				


				GLuint modelLoc = glGetUniformLocation(uiShaderProgram, "model");
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &engineTransformationComponent[i].worldTransformMatrix[0][0]);

				
				

				const UIVertexFormat uiVertexFormat = uiRenderMeshComponentPtr->vertexFormat;
				const  UIBufferType uiBuffertype = uiRenderMeshComponentPtr->bufferType;

				GPUBufferInfo vertexBufferInfo = uiVertexBufferManagementSystem.getBufferInfoForVertexFormat(uiVertexFormat, uiBuffertype);

			

				glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vertexBufferInfo.bufferHandle);
	
				

				//GLuint startIdxLoc = glGetUniformLocation(uiShaderProgram, "u_StartVertexIndex");
				//glUniform1ui(startIdxLoc, uiRenderMeshComponentPtr[i].vertexBufferOffset / 32);

			
			
				glDrawArrays(GL_TRIANGLES, uiRenderMeshComponentPtr[i].vertexBufferOffset / 32, uiRenderMeshComponentPtr[i].vertexCount);

	

			}


		}


	}




	void renderText()
	{






	}





}