#pragma once
#include "../glad/glad.h"
#include <string>


class ShaderManager
{
private:


public:
	ShaderManager();

	GLuint compileShader(const std::string& vertShaderCode,const std::string& fragShaderCode);
};