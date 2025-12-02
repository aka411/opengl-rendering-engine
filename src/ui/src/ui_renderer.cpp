#include "../../../include/ui/include/ui_renderer.h"
#include "../../../glad/glad.h"



namespace UI
{


	UIRenderer::UIRenderer(UICoreSystem& uiCoreSystem) : m_uiCoreSystem(uiCoreSystem)
	{





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


	}




	void renderText()
	{






	}





}