#pragma once
#include "rendering_system_data_types.h"
#include <map>
#include <string>
#include "shader_code.h"


static std::map<VertexAttributeType, std::string> vertexAttibuteToTypeString = {
{VertexAttributeType::POSITION_3F , "vec3"},
{VertexAttributeType::NORMAL_3F , "vec3"},

{VertexAttributeType::TEXCOORD_0_2F , "vec2"},
{VertexAttributeType::TEXCOORD_1_2F , "vec2"},
{VertexAttributeType::TEXCOORD_2_2F , "vec2"},
{VertexAttributeType::TEXCOORD_3_2F , "vec2"},

{VertexAttributeType::COLOR_0_4UB_NORMALIZED , "vec4"},
{VertexAttributeType::COLOR_1_4UB_NORMALIZED , "vec4"},
{VertexAttributeType::COLOR_2_4UB_NORMALIZED , "vec4"},
{VertexAttributeType::COLOR_3_4UB_NORMALIZED , "vec4"},

{VertexAttributeType::JOINT_INDICES_4I , "ivec4"},
{VertexAttributeType::JOINT_WEIGHTS_4F , "ivec4"},

{VertexAttributeType::TANGENT_4F , "vec4"},

};

static std::map<VertexAttributeType, std::string> vertexAttibuteToPresenceString = {
{VertexAttributeType::POSITION_3F , "HAS_POSITION"},
{VertexAttributeType::NORMAL_3F , "HAS_NORMAL"},

{VertexAttributeType::TEXCOORD_0_2F , "HAS_TEXCOORD_0"},
{VertexAttributeType::TEXCOORD_1_2F , "HAS_TEXCOORD_1"},
{VertexAttributeType::TEXCOORD_2_2F , "HAS_TEXCOORD_2"},
{VertexAttributeType::TEXCOORD_3_2F , "HAS_TEXCOORD_3"},

{VertexAttributeType::COLOR_0_4UB_NORMALIZED , "HAS_COLOR_0"},
{VertexAttributeType::COLOR_1_4UB_NORMALIZED , "HAS_COLOR_1"},
{VertexAttributeType::COLOR_2_4UB_NORMALIZED , "HAS_COLOR_2"},
{VertexAttributeType::COLOR_3_4UB_NORMALIZED , "HAS_COLOR_3"},

{VertexAttributeType::JOINT_INDICES_4I , "HAS_JOINT_INDICES"},
{VertexAttributeType::JOINT_WEIGHTS_4F , "HAS_JOINT_WEIGHTS"},

{VertexAttributeType::TANGENT_4F , "HAS_TANGENT"},

};




static std::map<VertexAttributeType, std::string> vertexAttibuteToNameString = {
{VertexAttributeType::POSITION_3F , "inPosition"},
{VertexAttributeType::NORMAL_3F , "inNormal"},

{VertexAttributeType::TEXCOORD_0_2F , "inTexCoord_0"},
{VertexAttributeType::TEXCOORD_1_2F , "inTexCoord_1"},
{VertexAttributeType::TEXCOORD_2_2F , "inTexCoord_2"},
{VertexAttributeType::TEXCOORD_3_2F , "inTexCoord_3"},

{VertexAttributeType::COLOR_0_4UB_NORMALIZED , "inColor_0"},
{VertexAttributeType::COLOR_1_4UB_NORMALIZED , "inColor_1"},
{VertexAttributeType::COLOR_2_4UB_NORMALIZED , "inColor_2"},
{VertexAttributeType::COLOR_3_4UB_NORMALIZED , "inColor_3"},


{VertexAttributeType::JOINT_INDICES_4I , "inJointIndices"},
{VertexAttributeType::JOINT_WEIGHTS_4F , "inJointWeights"},

{VertexAttributeType::TANGENT_4F , "inTangent"},

};






struct ShaderCode
{
	std::string vertexShaderCode;
	std::string fragmentShaderCode;
};

class ShaderCodeGenerator
{

private:

	struct ShaderFlagsAndAttributes
	{
		std::string shaderFlags = "";
		std::string shaderVertexAttributes = "";
	};

	ShaderFlagsAndAttributes getFlagsForAttributes(VertexFormat vertexFormat);

public:

	ShaderCode getShaderCodeForFormat(VertexFormat vertexFormat);



};