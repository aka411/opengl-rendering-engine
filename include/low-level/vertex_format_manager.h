#pragma once
#include <unordered_map>
#include "../../glad/glad.h"
#include "rendering_system_data_types.h"






class VertexFormatManager
{

private:

	std::unordered_map<VertexFormat,GLuint> m_vertexFormatToVAO;
	void createNewVAOForFormat(VertexFormat vertexFormat);
public:

	VertexFormatManager();

	

	//get VAO for given Vertex format
	GLuint getVAO(VertexFormat vertexFormat);



};