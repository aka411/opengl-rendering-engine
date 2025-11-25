#include "../../include/low-level/vertex_format_manager.h"
#include "../../include/low-level/vertex_format_helper.h"
#include <cassert>




const  std::map<ComponentDataType, GLenum> VertexFormatManager::COMPONENT_DATA_TYPE_TO_GL_TYPE_MAP =
{
	{ComponentDataType::FLOAT, GL_FLOAT },
	{ ComponentDataType::UNSIGNED_BYTE, GL_UNSIGNED_BYTE },
	{ ComponentDataType::UNSIGNED_SHORT, GL_UNSIGNED_SHORT }
	//{ComponentDataType::UNSIGNED_INT, GL_UNSIGNED_INT},
	//{ComponentDataType::INT, GL_INT},
	//{ComponentDataType::BYTE, GL_BYTE},
	//{ComponentDataType::SHORT, GL_SHORT},
	//{ComponentDataType::DOUBLE, GL_DOUBLE 

};

void VertexFormatManager::createNewVAOForFormat(VertexFormat vertexFormat)
{
	GLuint vao = 0;
	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);


	const auto vertexFormatAttributes = VertexFormatHelper::getVertexAttributeInfosForVertexFormat(vertexFormat);

	size_t location = 0;
	size_t offset = 0;

	for (const auto& vertexAttribute : vertexFormatAttributes)
	{

		const GLenum glType = getGLTypeFromComponentDataType(vertexAttribute.componentDataType);

		ComponentType componentType = vertexAttribute.componentType;
		ComponentDataType componentDataType = vertexAttribute.componentDataType;

		if (componentDataType != ComponentDataType::UNSIGNED_BYTE && componentDataType != ComponentDataType::UNSIGNED_SHORT) // need to look into this more
		{
			glVertexAttribFormat(location, VertexFormatHelper::getNumComponentsForComponentType(componentType), glType, false, offset);
		}
		else
		{

			glVertexAttribIFormat(location, VertexFormatHelper::getNumComponentsForComponentType(componentType), glType, offset);
		}

		glVertexAttribBinding(location, 0);
		glEnableVertexAttribArray(location);





		offset += vertexAttribute.sizeInBytes;
		location++;
	}




	m_vertexFormatToVAO[vertexFormat] = vao;

}


GLenum VertexFormatManager::getGLTypeFromComponentDataType(ComponentDataType componentDataType)
{
	const auto& it = COMPONENT_DATA_TYPE_TO_GL_TYPE_MAP.find(componentDataType);
	assert(it != COMPONENT_DATA_TYPE_TO_GL_TYPE_MAP.end() && "Unsupported ComponentDataType in getGLTypeFromComponentDataType");
	return it->second;

}




VertexFormatManager::VertexFormatManager()
{


}

GLuint VertexFormatManager::getVAO(VertexFormat vertexFormat)
{

	//create one if not 
	const auto& it = m_vertexFormatToVAO.find(vertexFormat);
	if (it == m_vertexFormatToVAO.end())
	{
		createNewVAOForFormat(vertexFormat);
	}



	//what if creation failed casue no bit was set?

	return m_vertexFormatToVAO.at(vertexFormat);

}
