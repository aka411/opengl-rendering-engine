#pragma once 
#include "buffer_management_system.h"
#include <string>
#include "shader_manager.h"
#include "camera.h"
//#include "gltf_parser.h"
#include "hierarchy_traverser_flat.h"
#include "vertex_format_manager.h"


class Renderer
{

private:

	struct ViewPort
	{
		int width = 0;
		int height = 0;
		float aspectRatio = 1.0f; // width / height

	};


	ViewPort m_viewPort;

	Engine::Camera& m_camera;


	BufferManagementSystem& m_bufferManagementSystem;
	ShaderManager m_shaderManager;
	GPUTextureManager& m_gpuTextureManager;

	VertexFormatManager m_vertexFormatManager;

	struct CameraShaderData
	{
		glm::mat4 projection;
		glm::mat4 view;
    };


	GLuint m_defaultShaderProgram;
	GLuint m_bonedShaderProgram;



	HierarchyTraverserFlat m_hierarchyTraverser;

	void setVirtualViewport(int width, int height);

public:

	Renderer(BufferManagementSystem& bufferManagementSystem, GPUTextureManager& gpuTextureManager,Engine::Camera& camera);



	void setPhysicalViewport(int width, int height);


    void renderFrame(Engine::Model& engineModel);

};

//get root , transverse set vertx buffer offset