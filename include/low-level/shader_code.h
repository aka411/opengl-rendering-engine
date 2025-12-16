#pragma once
#include <string>


//yup i know hardcoding shader code in header files is not a good idea, i will change this later








static std::string pbrNormalBaseVertexCode = R"(

#version 460 core
#extension GL_ARB_bindless_texture : require

#INSERT_FLAGS

#INSERT_VERTEX_ATTRIBUTES


/****PER FRAME DATA****/
layout(std140,binding = 0 ) uniform Camera
{
 mat4 projection;
 mat4 view;

} camera;







struct ObjectData
{
 mat4 modelMatrix;
 uint materialId;
 uint boneId;
vec2 padding;
};

// readonly SSBO containing the data
layout(binding = 0, std430) readonly buffer objectDataSSBO 
{

 ObjectData objectData[];

};


#ifdef HAS_JOINTS

// readonly SSBO containing the data
layout(binding = 1, std430) readonly buffer jointSSBO 
{

 mat4 joints[];

};


#endif





/*OUT_TO_FRAFMENT_SHADER*/

#ifdef HAS_TEXCOORD_0
out vec2 vs_texCoord_0;
#endif

#ifdef HAS_TEXCOORD_1
out vec2 vs_texCoord_1;
#endif

#ifdef HAS_TEXCOORD_2
out vec2 vs_texCoord_2;
#endif

#ifdef HAS_TEXCOORD_3
out vec2 vs_texCoord_3;
#endif



#ifdef HAS_TANGENT
out vec4 vs_tangent;
#endif



out vec3 vs_position;
out vec3 vs_normal;
flat out mat4 viewMatrix;

flat out uint vs_materialId;

void main()
{

ObjectData perObjectData = objectData[gl_BaseInstance];



vec4 position = vec4(inPosition, 1.0);

#ifdef HAS_JOINTS

const uint boneOffset = perObjectData.boneId;

    mat4 boneTransform = 
        inWeights.x * joints[boneOffset+inJoints.x] +
        inWeights.y * joints[boneOffset+inJoints.y] +
        inWeights.z * joints[boneOffset+inJoints.z] +
        inWeights.w * joints[boneOffset+inJoints.w];


    position = boneTransform * position;
#endif


 viewMatrix = camera.view;
 
 gl_Position = camera.projection * camera.view * perObjectData.modelMatrix * position;





 /** Outputs To Fragment Shader **/

 vs_materialId = perObjectData.materialId;

 vs_normal = normalize(mat3(camera.view * perObjectData.modelMatrix) * inNormal);
 vs_position = vec3((camera.view * perObjectData.modelMatrix * vec4( inPosition, 1.0)).xyz);


#ifdef HAS_TEXCOORD_0
vs_texCoord_0 = inTexCoord_0;
#endif

#ifdef HAS_TEXCOORD_1
vs_texCoord_1 = inTexCoord_1;
#endif

#ifdef HAS_TEXCOORD_2
vs_texCoord_2 = inTexCoord_2;
#endif

#ifdef HAS_TEXCOORD_3
vs_texCoord_3 = inTexCoord_3;
#endif


#ifdef HAS_TANGENT
vs_tangent = inTangent;
#endif





}






)";



static std::string pbrNormalBaseFragmentCode = R"(
#version 460 core
//#extension GL_ARB_gpu_shader_int64 : require //not supported by my graphics card
#extension GL_ARB_bindless_texture : require


#INSERT_FLAGS



// ---  Presence Flags (Bits 0-4) ---
    #define HAS_ALBEDO_TEX      1u << 0  // Base Color Texture
    #define HAS_MR_TEX          1u << 1  // Metallic Roughness Texture
    #define HAS_NORMAL_TEX      1u << 2  // Normal Texture
    #define HAS_OCCLUSION_TEX   1u << 3  // Occlusion Texture
    #define HAS_EMISSIVE_TEX    1u << 4  // Emissive Texture

    // ---  Texture Coordinate Index Shifts (Bits 5-14) ---
    // These define the starting bit position for a 2-bit field (0, 1, 2 or 3) upto 3 texture coordinates.
    // The actual value (0 or 1 ..3) will be extracted using bitwise operations.

    #define ALBEDO_TEXCOORD_SHIFT    5
    #define MR_TEXCOORD_SHIFT        8
    #define NORMAL_TEXCOORD_SHIFT    11
    #define OCCLUSION_TEXCOORD_SHIFT 14
    #define EMISSIVE_TEXCOORD_SHIFT  17



//Note : Need to add support for 5 texture coordinates







struct PBRMetallicRoughnessMaterial 
{

	vec4 baseColorFactor; 
	vec3 emissiveFactor; 

	float metallicFactor ;
	float roughnessFactor; 



	uvec2 albedoTextureHandle;
	uvec2 metallicRoughnessTextureHandle; 

	uvec2 normalTextureHandle ; 
	uvec2 occlusionTextureHandle;
	uvec2 emissiveTextureHandle ;

	uvec2 materialBitMask; 

};


layout(std430, binding = 2) readonly buffer MaterialData 
{
    PBRMetallicRoughnessMaterial materials[];
};














// --- PBR Utility Functions  ---



const float PI = 3.14159265359;


// Distribution (D) term: Trowbridge-Reitz GGX
float distributionGGX(vec3 N, vec3 H, float roughness) 
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom; // Assuming PI is defined somewhere

    return a2 / denom;
}

// Geometry (G) term: Schlick-GGX
float geometrySchlickGGX(float NdotV, float roughness) 
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}

// Geometry term: Smith function (combining V and L components)
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) 
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel (F) term: Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0)
 {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
 }





/*** IN FROM VERTEX SHADER ****/


#ifdef HAS_TEXCOORD_0
in vec2 vs_texCoord_0;
#endif

#ifdef HAS_TEXCOORD_1
in vec2 vs_texCoord_1;
#endif

#ifdef HAS_TEXCOORD_2
in vec2 vs_texCoord_2;
#endif

#ifdef HAS_TEXCOORD_3
in vec2 vs_texCoord_3;
#endif


#ifdef HAS_TANGENT
in vec4 vs_tangent;
#endif



flat in uint vs_materialId;

in vec3 vs_normal;
in vec3 vs_position;
flat in mat4 viewMatrix;


vec2 getTexCoord(int index) 
{
    #ifdef HAS_TEXCOORD_0
    if (index == 0) return vs_texCoord_0;
    #endif

    #ifdef HAS_TEXCOORD_1
    if (index == 1) return vs_texCoord_1;
    #endif

    #ifdef HAS_TEXCOORD_2
    if (index == 2) return vs_texCoord_2;
    #endif

    #ifdef HAS_TEXCOORD_3
    if (index == 3) return vs_texCoord_3;
    #endif

    return vs_texCoord_0; 
}

/*** OUT TO FRAMEBUFFER ***/
out vec4 FragColor;


void main()
{


PBRMetallicRoughnessMaterial material = materials[vs_materialId];

const int albedoTexturePresent = int( material.materialBitMask.x & HAS_ALBEDO_TEX);
const int mrTexturePresent       = int(material.materialBitMask.x & HAS_MR_TEX);
const int normalTexturePresent   =  int(material.materialBitMask.x & HAS_NORMAL_TEX);
const int occlusionTexturePresent =  int(material.materialBitMask.x & HAS_OCCLUSION_TEX);
const int emissiveTexturePresent  =  int(material.materialBitMask.x & HAS_EMISSIVE_TEX);


const int albedoTexCoordIndex  =  int((material.materialBitMask.x >> ALBEDO_TEXCOORD_SHIFT) & 3);
const int mrTexCoordIndex        = int( (material.materialBitMask.x >> MR_TEXCOORD_SHIFT) & 3);
const int normalTexCoordIndex    =  int((material.materialBitMask.x >> NORMAL_TEXCOORD_SHIFT) & 3);
const int occlusionTexCoordIndex =  int((material.materialBitMask.x >> OCCLUSION_TEXCOORD_SHIFT) & 3);
const int emissiveTexCoordIndex  =  int((material.materialBitMask.x >> EMISSIVE_TEXCOORD_SHIFT) & 3);






    vec4 baseColor = material.baseColorFactor;

    float metallic = material.metallicFactor;
    float roughness =  material.roughnessFactor;

    vec3 F0 = vec3(0.04);
    float occlusion = 1.0;

    vec3 emissive = material.emissiveFactor;

    

    // ****** Albedo Texture Fetch ******//

    if (albedoTexturePresent != 0) {
      

        vec2 uv = getTexCoord(albedoTexCoordIndex);
        
        baseColor *= texture2D(sampler2D(material.albedoTextureHandle), uv);
    }
    


    // ********Metallic Roughness Texture Fetch ****//

    if (mrTexturePresent != 0) 
   {
        vec2 uv = getTexCoord(mrTexCoordIndex);
        vec4 mrSample = texture2D(sampler2D(material.metallicRoughnessTextureHandle), uv);
        
        // Gltf packs metallic into B and roughness into G channel
        metallic *= mrSample.b;
        roughness *= mrSample.g;
    }
    
    // --- Occlusion Texture Fetch ---
    if (occlusionTexturePresent != 0) 
    {
        vec2 uv = getTexCoord(occlusionTexCoordIndex);
        // Occlusion is stored in the Red channel (R)
        occlusion = texture2D(sampler2D(material.occlusionTextureHandle), uv).r;
    }

    // --- Emissive Texture Fetch ---
    if (emissiveTexturePresent != 0) 
{
        vec2 uv = getTexCoord(emissiveTexCoordIndex);
        // Emissive color is multiplied by the texture sample
        emissive *= texture2D(sampler2D(material.emissiveTextureHandle), uv).rgb;
    }



    // ********** Normal Map Fetch (TBN basis)********//

vec3 finalNormal;


 #ifdef HAS_TANGENT
    if (normalTexturePresent != 0) 
    {

        
        //  Get the TBN frame
        vec3 T = normalize(vs_tangent.xyz);
        vec3 N_in = normalize(vs_normal);
       
        
        //  glTF  handedness:
        vec3 B = (vs_tangent.w > 0.0) ? cross(N_in, T) : -cross(N_in, T);

        mat3 tbn = mat3(T, B, N_in);
        
 
        vec2 uv = getTexCoord(normalTexCoordIndex);
        vec3 normalSample = texture2D(sampler2D(material.normalTextureHandle), uv).rgb;


        normalSample = normalize(normalSample * 2.0 - 1.0); 


       finalNormal  = normalize(tbn * normalSample);

    }
#else

finalNormal = vs_normal;

#endif



/*****LIGHTING  CALCULATIONS******/





const vec3 LIGHT_DIRECTION_WORLD = normalize(vec3(0.5, -0.8, 0.2));
const vec3 LIGHT_COLOR = vec3(1.0, 1.0, 1.0); 
const vec3 CAMERA_POS = vec3(0.0, 0.0, 0.0); //camera always at origin in view space




vec3 lightDirView = mat3(viewMatrix) * LIGHT_DIRECTION_WORLD;


vec3 L = -normalize(lightDirView);
vec3 V = normalize(CAMERA_POS - vs_position); // View vector (points from fragment to camera)


vec3 N = finalNormal; // Use the TBN-transformed normal

vec3 F0_final = mix(F0, baseColor.rgb, metallic); 


vec3 H = normalize(V + L); // Halfway vector
float NdotL = max(dot(N, L), 0.0);

// --- PBR BRDF Terms ---

float D = distributionGGX(N, H, roughness);
float G = geometrySmith(N, V, L, roughness);
vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0_final);



vec3 kS = F;
vec3 kD = vec3(1.0) - kS; 
kD *= 1.0 - metallic; 

vec3 numerator   = D * G * F;
float denominator = 4.0 * max(dot(N, V), 0.0) * NdotL + 0.001; 
vec3 specular = numerator / denominator;


vec3 albedo = baseColor.rgb;
vec3 diffuse = (kD * albedo) / PI;




// --- Final Lighting Composition ---


// Light contribution
vec3 Lo = (diffuse + specular) * LIGHT_COLOR * NdotL;


vec3 ambient = vec3(0.1) * albedo * occlusion;

// Final color
vec3 finalColor = ambient + Lo + emissive;



 FragColor = vec4(finalColor, baseColor.a);



}




)";
