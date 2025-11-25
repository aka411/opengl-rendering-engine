#pragma once
#include <unordered_map>
#include "../../glad/glad.h"
#include "rendering_system_data_types.h"
#include <map>





class VertexFormatManager
{

private:


	const static std::map<ComponentDataType, GLenum> COMPONENT_DATA_TYPE_TO_GL_TYPE_MAP;

	std::unordered_map<VertexFormat,GLuint> m_vertexFormatToVAO;
	void createNewVAOForFormat(VertexFormat vertexFormat);


	static GLenum getGLTypeFromComponentDataType(ComponentDataType componentDataType);

public:

	VertexFormatManager();

	

	//get VAO for given Vertex format
	GLuint getVAO(VertexFormat vertexFormat);



};