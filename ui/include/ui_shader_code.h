#pragma once
#include <string>


std::string normalUIShaderVertexCode = R"(
#version 460 core

layout (location = 0) in vec3 inPosition;




uniform mat4 projection;

uniform mat4 model;

 


//out vec4 fs_color;

void main()
{


 gl_Position = projection * model * vec4(inPosition,1.0f);

}





)";


std::string normalUIShaderFragmentCode = R"(
#version 460 core



out vec4 FragColor;


void main()
{
	FragColor = vec4(1.0f,0.2f,0.0f,1.0f);
}

)";





/*Text Shader Code*/

std::string textUIShaderVertexCode = R"(




)";

std::string textUIShaderFragmentCode = R"(
)";