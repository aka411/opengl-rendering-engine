#pragma once
#include <vector>
#include "../glad/glad.h"

class VertexFormatManager
{
private:
	std::vector<GLuint>  m_allocatedVAOs;

	GLuint m_defaultVAO;
	GLuint m_bonedVAO;

public:
	VertexFormatManager();
	GLuint getDefaultVAO();
	GLuint getBonedVAO();

};