#pragma once
#include <string>


std::string normalUIShaderVertexCode = R"(
#version 460 core


//layout (location = 0) in vec4 inPosition;
//layout(location = 1) in vec4 color;




struct VertexData 
{
    vec4 positionIn;
    vec4 colorIn;

};

// readonly SSBO containing the data
layout(binding = 0, std430) readonly buffer ssbo1 
{
    VertexData data[];
};



uniform mat4 projection;

uniform mat4 model;

 


out vec4 vs_color;

void main()
{

 
 gl_Position =  projection * model * vec4(data[gl_VertexID].positionIn);

 vs_color = vec4(data[gl_VertexID].colorIn);
}





)";


std::string normalUIShaderFragmentCode = R"(
#version 460 core



in vec4 vs_color;

out vec4 FragColor;

void main()
{
	FragColor = vs_color;
}

)";





/*Text Shader Code*/

std::string textUIShaderVertexCode = R"(




)";

std::string textUIShaderFragmentCode = R"(
)";