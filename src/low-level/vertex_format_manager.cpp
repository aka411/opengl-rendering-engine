#include "../../include/low-level/vertex_format_manager.h"
#include "../../include/low-level/vertex_format_helper.h"
#include <cassert>








void VertexFormatManager::createNewVAOForFormat(VertexFormat vertexFormat)
{
	GLuint vao;
	size_t location = 0;
	size_t offset = 0;
	for (uint32_t i = 0; i < vertexFormat.size(); ++i)
	{
		if (vertexFormat.test(i))
		{
			assert(i != 0);
			VertexAttribute vertexAttribute = VertexFormatHelper::GetVertexAttributeDetails(static_cast<VertexAttributeType>(i));

			//ToDo : add creation logic here



		}
	}



	m_vertexFormatToVAO[vertexFormat] = vao;

}

VertexFormatManager::VertexFormatManager()
{


}

GLuint VertexFormatManager::getVAO(VertexFormat vertexFormat)
{

	//create one if not 
	const auto& it = m_vertexFormatToVAO.find(vertexFormat);
	if (it != m_vertexFormatToVAO.end())
	{
		return it->second;
	}
	

	createNewVAOForFormat(vertexFormat);
	//what if creation failed casue no bit was set?

	return m_vertexFormatToVAO.at(vertexFormat);

}
