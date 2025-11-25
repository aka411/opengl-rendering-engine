#pragma once
#include "../../glad/glad.h"
#include <string>
#include <unordered_map>
#include "rendering_system_data_types.h"
#include "shader_code_generator.h"

enum class ShaderType
{
	PBR_NORMAL,
	PBR_SKINNED

};

enum class ShaderStage
{
	VERTEX,
	FRAGMENT
};


class ShaderManager
{
private:
	std::unordered_map<VertexFormat, GLuint> m_vertexFormatToShaderProgram;
	ShaderCodeGenerator m_shaderCodeGenerator;

		GLuint compileShader(const std::string& vertShaderCode, const std::string& fragShaderCode);
		void createNewShaderProgramForFormat(VertexFormat vertexFormat);
public:
	ShaderManager();

	

	GLuint getShaderProgramForVertexFormat(VertexFormat vertexFormat);
};