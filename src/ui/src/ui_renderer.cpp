#include "../../../include/ui/include/ui_renderer.h"
#include "../../../glad/glad.h"
#include "../../../include/ui/include/core/ui_component.h"
#include "../../../include/components.h"



namespace UI
{


	UIRenderer::UIRenderer(UICoreSystem& uiCoreSystem) : m_uiCoreSystem(uiCoreSystem)
	{

		
		glCreateVertexArrays(1, &m_dummyVAO);
		//Need valid vao binded for vertx pulling to work


		m_uiCamera.setOrthographicProjection(0, 1000, 700, 0, 10, -100);

	}


	void UIRenderer::renderUI()
	{

		/****PER FRAME STATE****/
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);

		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		glDisable(GL_CULL_FACE);//WARNING : This is done cause in our UI 
		//system the Y is down and we do this by flipping Y Axis in camera parameters
		//But since we flipped the Y in camera matrix and when we use the projection matrix in
		//shader it causes the winding order to become CW from CCW.
		//So for now disabling culling is a temp fix.

		glEnable(GL_BLEND);
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray(m_dummyVAO);//Even though we are using 
		//vertex pulling we need a dummy VAO binded for it to work
		



		/**PER FORMAT STATE***/

		GLuint uiShaderProgram = m_uiCoreSystem.getUIShaderManager().getShaderProgramForVertexFormat(UIVertexFormat::NORMAL_UI_VERTEX_FORMAT);


		glUseProgram(uiShaderProgram);



		GLuint projectionLoc = glGetUniformLocation(uiShaderProgram, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &m_uiCamera.getProjectionMatrix()[0][0]);

	
		ECS::ECSEngine& ecsEngine = m_uiCoreSystem.getECSEngine();

		ECS::Query query = ecsEngine.getQuery<UIRenderMeshComponent,EngineTransformationComponent>();


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
	
				

		

			
			
				glDrawArrays(GL_TRIANGLES, uiRenderMeshComponentPtr[i].vertexBufferOffset / 32, uiRenderMeshComponentPtr[i].vertexCount);

	

			}


		}


	}




	void renderText()
	{






	}





}