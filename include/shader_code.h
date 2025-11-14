#pragma once
#include <string>


//yup i know hardcoding shader code in header files is not a good idea, i will change this later

std::string defaultVertexShaderCode = R"(
#version 450 core
layout(location = 0 ) in vec3 inPosition;
layout(location = 1 ) in vec3 inNormal;
layout(location = 2 ) in vec2 inTexCoord;


layout(std140,binding = 0 ) uniform Camera
{
 mat4 projection;
 mat4 view;

};


//per object
layout(location = 5 ) uniform mat4 model;



out vec3 vertexNormal; 
out vec2 texCoord;


void main()
{
    vertexNormal = mat3(model) * inNormal;
	texCoord = inTexCoord;

    gl_Position = projection * view * model * vec4( inPosition, 1.0); 

 
}



)";


std::string defaultFragmentShaderCode = R"(
#version 450 core


layout (std140, binding = 5) uniform material
{
    vec4 baseColorFactor;
    vec3 emmissiveFactor;
    float metallicFactor;
    float roughnessFactor;

	bool baseColorTexturePresent;
	bool metallicRoughnessTexturePresent;

	bool normalTexturePresent;
	bool occulsionTexturePresent;
	bool emmissiveTexturePresent;
};

in vec3 vertexNormal;
in vec2 texCoord;

out vec4 FragColor; 





layout (binding = 6) uniform sampler2D u_BaseColorTexture;     // Texture Unit 1
layout (binding = 7) uniform sampler2D u_MetallicRoughnessTexture; // Texture Unit 2

layout (binding = 8) uniform sampler2D u_NormalTexture;          // Texture Unit 3
layout (binding = 9) uniform sampler2D u_OcclusionTexture;       // Texture Unit 4
layout (binding = 10) uniform sampler2D u_EmissiveTexture;        // Texture Unit 5




void main() {

vec3 sunlightDir = normalize(vec3(-0.7f, -0.7f, -0.2f));
vec3 AMBIENT_LIGHT = vec3(0.15, 0.15, 0.15);
    vec4 baseColor = baseColorFactor;
    float metallic = metallicFactor;
    float roughness = roughnessFactor;
    
 
    float ambientOcclusion = 1.0; 



    vec3 emissive = emmissiveFactor.rgb; 


    // 2. Base Color (Albedo)
    if (baseColorTexturePresent) {

        baseColor = texture(u_BaseColorTexture, texCoord); 

    }

    // 3. Metallic-Roughness (Channel Packed)

    if (metallicRoughnessTexturePresent) {
        // glTF standard:
        // Metallic/Roughness is combined into one texture.
        // G channel = Roughness
        // B channel = Metallic
        vec4 mr = texture(u_MetallicRoughnessTexture, texCoord);
        

        roughness *= mr.g; 
        metallic *= mr.b;
    }


    if (occulsionTexturePresent) {

        ambientOcclusion = texture(u_OcclusionTexture, texCoord).r; 
    }

    if (emmissiveTexturePresent) {
        // Emissive texture is sRGB and multiplies the emissive factor.
        emissive *= texture(u_EmissiveTexture, texCoord).rgb;
    }
    

    vec3 N = normalize(vertexNormal);
    


   
    float NdotL = max(0.0, dot(N, -sunlightDir));
    vec3 lightColor = vec3(1.0, 1.0, 1.0) * 0.9;

   vec3 diffuseLight = lightColor * NdotL;
    
    vec3 ambientTerm = baseColor.rgb * AMBIENT_LIGHT;

   
    
    vec3 directLight = baseColor.rgb * diffuseLight;


    vec3 final_color = ambientTerm + directLight;


    
    
    final_color *= ambientOcclusion; // Apply Ao to the total light (not just ambient)

    final_color += emissive;

    FragColor = vec4(final_color, baseColor.a);

}
)";



    std::string bonedVertexShaderCode = R"(
#version 450 core
layout(location = 0 ) in vec3 inPosition;
layout(location = 1 ) in vec3 inNormal;
layout(location = 2 ) in vec2 inTexCoord;

layout(location = 3 ) in ivec4 inJoints; 
layout(location = 4 ) in vec4 inWeights;

layout(std140,binding = 0 ) uniform Camera
{
 mat4 projection;
 mat4 view;

};


//per object
layout(std140,binding = 1 ) uniform Bones
{
 mat4 joints[100];

};




layout(location = 5 ) uniform mat4 model;



out vec3 vertexNormal; 
out vec2 texCoord;


void main()
{



    mat4 boneTransform = 
        inWeights.x * joints[inJoints.x] +
        inWeights.y * joints[inJoints.y] +
        inWeights.z * joints[inJoints.z] +
        inWeights.w * joints[inJoints.w];



    vec4 finalPosition = model * boneTransform * vec4(inPosition, 1.0);
    gl_Position = projection * view * model * finalPosition;


    mat3 normalMatrix = mat3(model) * mat3(boneTransform);
    vertexNormal = normalMatrix * inNormal;



    texCoord = inTexCoord ;


}



)";