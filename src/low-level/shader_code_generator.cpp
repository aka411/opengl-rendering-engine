
#include "../../include/low-level/shader_code_generator.h"
#include <vector>
#include "../../include/low-level/vertex_format_helper.h"


const  std::map<VertexAttributeType, std::string> ShaderCodeGenerator::VERTEX_ATTRIBUTE_TO_SHADER_ATTRIBUTE_STRING =
{
    {VertexAttributeType::POSITION, "Position"},
    {VertexAttributeType::NORMAL, "Normal"},
    {VertexAttributeType::TANGENT, "Tangent"},

    {VertexAttributeType::TEXCOORD, "TexCoord"},
    {VertexAttributeType::COLOR, "Color"},

    {VertexAttributeType::JOINTS, "Joints"},
    {VertexAttributeType::WEIGHTS, "Weights"}
};

const  std::map<VertexAttributeType, std::string> ShaderCodeGenerator::VERTEX_ATTRIBUTE_TO_PRESENCE_FLAG_STRING =
{
    {VertexAttributeType::POSITION, "HAS_POSITION"},
    {VertexAttributeType::NORMAL, "HAS_NORMAL"},
    {VertexAttributeType::TANGENT, "HAS_TANGENT"},
    {VertexAttributeType::TEXCOORD, "HAS_TEXCOORD"},
    {VertexAttributeType::COLOR, "HAS_COLOR"},
    {VertexAttributeType::JOINTS, "HAS_JOINTS"},
    {VertexAttributeType::WEIGHTS, "HAS_WEIGHTS"}
};


const std::map<ComponentDataType, std::string> ShaderCodeGenerator::COMPONENT_DATA_TYPE_TO_SHADER_COMPONENT_DATA_TYPE_STRING =
{
    {ComponentDataType::FLOAT, "float"},
    {ComponentDataType::UNSIGNED_BYTE, "uint"},
    {ComponentDataType::UNSIGNED_SHORT, "uint"}
   // {ComponentDataType::UNSIGNED_INT, "uint"},
   // {ComponentDataType::INT, "int"},
   // {ComponentDataType::BYTE, "int"},
   // {ComponentDataType::SHORT, "int"},
   // {ComponentDataType::DOUBLE, "double"}
};

const  std::map<ComponentDataType, std::string> ShaderCodeGenerator::COMPONENT_DATA_TYPE_TO_SHADER_COMPONENT_DATA_TYPE_PREFIX_STRING =
{
    {ComponentDataType::FLOAT, ""},
    {ComponentDataType::UNSIGNED_BYTE, "u"},
    {ComponentDataType::UNSIGNED_SHORT, "u"}
    // {ComponentDataType::UNSIGNED_INT, "u"},
    // {ComponentDataType::INT, ""},
    // {ComponentDataType::BYTE, ""},
    // {ComponentDataType::SHORT, ""},
    // {ComponentDataType::DOUBLE, ""}
};





const std::map<ComponentType, std::string> ShaderCodeGenerator::COMPONENT_TYPE_TO_SHADER_COMPONENT_TYPE_STRING =
{
    {ComponentType::VEC2, "vec2"},
    {ComponentType::VEC3, "vec3"},
    {ComponentType::VEC4, "vec4"}
};






ShaderCodeGenerator::ShaderFlagsAndAttributes ShaderCodeGenerator::getFlagsForAttributes(VertexFormat vertexFormat)
{

    ShaderFlagsAndAttributes shaderFlagsAndAttributes;
    
    size_t location = 0;
    /*
    for (std::size_t i = 0; i < vertexFormat.size(); ++i)
    {
        VertexAttributeType vertexAttributeType = static_cast<VertexAttributeType>(i);
        if (vertexFormat.test(i))
        {
            if (vertexAttibuteToPresenceString.find(vertexAttributeType) != vertexAttibuteToPresenceString.end())
            {
                shaderFlagsAndAttributes.shaderFlags.append("#define").append(" ").append(vertexAttibuteToPresenceString[vertexAttributeType]).append("\n");

                shaderFlagsAndAttributes.shaderVertexAttributes
                    .append("layout(location = ")
                    .append(std::to_string(location)) // Convert the int 'location' to a string
                    .append(") in ")
                    .append(vertexAttibuteToTypeString.at(vertexAttributeType))
                    .append(" ")
                    .append(vertexAttibuteToNameString.at(vertexAttributeType))
                    .append(";")
                    .append("\n");


                location++;

            }
        }
    }*/



	const std::vector<VertexAttributeInfo> vertexAttributeInfos = VertexFormatHelper::getVertexAttributeInfosForVertexFormat(vertexFormat);

    for (const auto& attributeInfo : vertexAttributeInfos)
    {
        VertexAttributeType vertexAttributeType = attributeInfo.vertexAttributeType;
        ComponentDataType componentDataType = attributeInfo.componentDataType;
        ComponentType componentType = attributeInfo.componentType;

        std::string indexString = "";
        if (attributeInfo.index >= 0)
        {
            indexString = "_" + std::to_string(attributeInfo.index);
        }

        if (VERTEX_ATTRIBUTE_TO_PRESENCE_FLAG_STRING.find(vertexAttributeType) != VERTEX_ATTRIBUTE_TO_PRESENCE_FLAG_STRING.end())
        {
            shaderFlagsAndAttributes.shaderFlags.append("#define").append(" ").append(VERTEX_ATTRIBUTE_TO_PRESENCE_FLAG_STRING.at(vertexAttributeType)).append(indexString).append("\n");
            shaderFlagsAndAttributes.shaderVertexAttributes
                .append("layout(location = ")
                .append(std::to_string(location)) // Convert the int 'location' to a string
                .append(") in ")
                .append(COMPONENT_DATA_TYPE_TO_SHADER_COMPONENT_DATA_TYPE_PREFIX_STRING.at(componentDataType))
                .append(COMPONENT_TYPE_TO_SHADER_COMPONENT_TYPE_STRING.at(componentType))
                .append(" in")
                .append(VERTEX_ATTRIBUTE_TO_SHADER_ATTRIBUTE_STRING.at(vertexAttributeType))
                .append(indexString)
                .append(";")
                .append("\n");




        }
		++location;

    }


    return shaderFlagsAndAttributes;
}

ShaderCode ShaderCodeGenerator::getShaderCodeForFormat(VertexFormat vertexFormat)
{
    ShaderFlagsAndAttributes shaderFlagsAndAttributes = getFlagsForAttributes(vertexFormat);
    ShaderCode shaderCode;

    auto replaceString = [](std::string srcString, const std::string& searchString, const std::string& stringToBeReplacedWith)
        {
            std::string& modifiedShaderCode = srcString;
           
            size_t pos = modifiedShaderCode.find(searchString);

            if (pos != std::string::npos)
            {
                modifiedShaderCode.replace(pos, searchString.length(), stringToBeReplacedWith);
            }
            return modifiedShaderCode;
        };
        
  


    shaderCode.vertexShaderCode = replaceString(pbrNormalBaseVertexCode,"#INSERT_FLAGS", shaderFlagsAndAttributes.shaderFlags);
    shaderCode.vertexShaderCode = replaceString(shaderCode.vertexShaderCode, "#INSERT_VERTEX_ATTRIBUTES", shaderFlagsAndAttributes.shaderVertexAttributes);

    shaderCode.fragmentShaderCode = replaceString(pbrNormalBaseFragmentCode,"#INSERT_FLAGS", shaderFlagsAndAttributes.shaderFlags);

    return shaderCode;

}
