#include "../../include/low-level/vertex_format_manager.h"
#include "../../include/low-level/vertex_format_helper.h"
#include <cassert>








void VertexFormatManager::createNewVAOForFormat(VertexFormat vertexFormat)
{
	GLuint vao = 0;
	size_t location = 0;
	size_t offset = 0;
	


	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);

	for (uint32_t i = 0; i < vertexFormat.size(); ++i)
	{
		if (vertexFormat.test(i))
		{
			assert(i != 0);
			VertexAttribute vertexAttribute = VertexFormatHelper::GetVertexAttributeDetails(static_cast<VertexAttributeType>(i));


			
			

			if (!vertexAttribute.normalized) // no checking for integer type and normalize is needed // we need to improve this 
			{
				glVertexAttribFormat(location, vertexAttribute.count, vertexAttribute.glType, vertexAttribute.normalized, offset);
			}
			else
			{
				
				glVertexAttribIFormat(location, vertexAttribute.count, vertexAttribute.glType, offset);
			}

			glVertexAttribBinding(location, 0);
			glEnableVertexAttribArray(location);





			offset += vertexAttribute.size;
			location++;
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
	if (it == m_vertexFormatToVAO.end())
	{
		createNewVAOForFormat(vertexFormat);
	}
	


	//what if creation failed casue no bit was set?

	return m_vertexFormatToVAO.at(vertexFormat);

}
