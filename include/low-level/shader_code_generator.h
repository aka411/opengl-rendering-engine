#pragma once
#include "rendering_system_data_types.h"
#include <map>
#include <string>
#include "shader_code.h"








struct ShaderCode
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
};

class ShaderCodeGenerator
{

private:

	const static std::map<VertexAttributeType, std::string> VERTEX_ATTRIBUTE_TO_SHADER_ATTRIBUTE_STRING;
	const static std::map<VertexAttributeType, std::string> VERTEX_ATTRIBUTE_TO_PRESENCE_FLAG_STRING;

	const static std::map<ComponentDataType, std::string> COMPONENT_DATA_TYPE_TO_SHADER_COMPONENT_DATA_TYPE_STRING;
	const static std::map<ComponentDataType, std::string> COMPONENT_DATA_TYPE_TO_SHADER_COMPONENT_DATA_TYPE_PREFIX_STRING;

	const static std::map<ComponentType, std::string> COMPONENT_TYPE_TO_SHADER_COMPONENT_TYPE_STRING;




	struct ShaderFlagsAndAttributes
	{
		std::string shaderFlags = "";
		std::string shaderVertexAttributes = "";
	};

	ShaderFlagsAndAttributes getFlagsForAttributes(VertexFormat vertexFormat);

public:

	ShaderCode getShaderCodeForFormat(VertexFormat vertexFormat);



};