#include "../include/vertex_format_manager.h"

VertexFormatManager::VertexFormatManager()
{
	

	//set up VAO
	glCreateVertexArrays(1, &m_defaultVAO);
	glBindVertexArray(m_defaultVAO);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // postion attribute
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float))); // normal attribute
	glVertexAttribBinding(1, 0);
	glEnableVertexAttribArray(1);


	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // uv coords attribute
	glVertexAttribBinding(2, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);




	glCreateVertexArrays(1, &m_bonedVAO);
	glBindVertexArray(m_bonedVAO);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // postion attribute
	glVertexAttribBinding(0, 0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float))); // normal attribute
	glVertexAttribBinding(1, 0);
	glEnableVertexAttribArray(1);


	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // uv coords attribute
	glVertexAttribBinding(2, 0);
	glEnableVertexAttribArray(2);


	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // uv coords attribute
	glVertexAttribBinding(3, 0);
	glEnableVertexAttribArray(3);//weights

	glVertexAttribBinding(4, 0);//jointIndex
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
