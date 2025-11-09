#include "../include/vertex_format_manager.h"

VertexFormatManager::VertexFormatManager()
{
	

	//set up VAO
	glCreateVertexArrays(1, &m_defaultVAO);
	glBindVertexArray(m_defaultVAO);


	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(0);


	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 12);
	glVertexAttribBinding(1, 0);
	glEnableVertexAttribArray(1);
	


	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 24);
	glVertexAttribBinding(2, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);



	
	glCreateVertexArrays(1, &m_bonedVAO);

	glBindVertexArray(m_bonedVAO);


	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);//POSITION
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(0);


	glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 12);//NORMAL
	glVertexAttribBinding(1, 0);
	glEnableVertexAttribArray(1);



	glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 24);//TEXCOORD
	glVertexAttribBinding(2, 0);
	glEnableVertexAttribArray(2);



	glVertexAttribIFormat(3, 4, GL_INT, 32);//JOINT 16 bytes
	glVertexAttribBinding(3, 0);
	glEnableVertexAttribArray(3);


	glVertexAttribFormat(4, 4, GL_FLOAT, GL_FALSE, 48);//WEIGHTS
	glVertexAttribBinding(4, 0);
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);




}

GLuint VertexFormatManager::getDefaultVAO()
{
	return m_defaultVAO;
}

GLuint VertexFormatManager::getBonedVAO()
{
	return m_bonedVAO;
}
