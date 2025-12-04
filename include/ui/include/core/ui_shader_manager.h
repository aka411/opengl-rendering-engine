#pragma once
#include "../../../../glad/glad.h"
#include "ui_core_data_structures.h"


namespace UI
{

	class UIShaderManager
	{

	private:

		std::unordered_map<UIVertexFormat, GLuint> m_vertexFormatToShaderProgram;


		GLuint compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode);

	public:

		UIShaderManager();

	
		GLuint getShaderProgramForVertexFormat(UIVertexFormat uiVertexFormat);

	};


}