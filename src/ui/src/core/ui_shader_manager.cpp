#include "../../../../include/ui/include/core/ui_shader_manager.h"
#include <iostream>
#include "../../../../include/ui/include/core/ui_shader_code.h"


namespace UI
{


	GLuint UIShaderManager::compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode)
	{

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertShaderSource = vertShaderCode.c_str();
		const char* fragShaderSource = fragShaderCode.c_str();

		glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
		glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);



		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// Check for fragment shader compilation errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}



		GLuint shaderProgram;

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		//cleanup
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);


		return shaderProgram;

	}


	UIShaderManager::UIShaderManager()
	{

		m_vertexFormatToShaderProgram[UIVertexFormat::NORMAL_UI_VERTEX_FORMAT] = compileShader(normalUIShaderVertexCode, normalUIShaderFragmentCode);
		//m_vertexFormatToShaderProgram[UIVertexFormat::TEXT_UI_VERTEX_FORMAT] = compileShader();
	}

	GLuint UIShaderManager::getShaderProgramForVertexFormat(UIVertexFormat uiVertexFormat)
	{


		return m_vertexFormatToShaderProgram.at(uiVertexFormat);
	
	}






}