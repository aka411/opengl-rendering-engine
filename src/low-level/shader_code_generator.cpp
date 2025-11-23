
#include "../../include/low-level/shader_code_generator.h"




ShaderCodeGenerator::ShaderFlagsAndAttributes ShaderCodeGenerator::getFlagsForAttributes(VertexFormat vertexFormat)
{

    ShaderFlagsAndAttributes shaderFlagsAndAttributes;
    
    size_t location = 0;
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
    }

    return shaderFlagsAndAttributes;
}

ShaderCode ShaderCodeGenerator::getShaderCodeForFormat(VertexFormat vertexFormat)
{
    ShaderFlagsAndAttributes shaderFlagsAndAttributes = getFlagsForAttributes(vertexFormat);
    ShaderCode shaderCode;

    auto replaceString = [](std::string& srcString, const std::string& searchString, const std::string& replaceString)
        {
            std::string& modifiedShaderCode = srcString;
           
            size_t pos = modifiedShaderCode.find(searchString);

            if (pos != std::string::npos)
            {
                modifiedShaderCode.replace(pos, searchString.length(), replaceString);
            }
            return modifiedShaderCode;
        };
        
  


    shaderCode.vertexShaderCode = replaceString(pbrNormalBaseVertexCode,"#INSERT_FLAGS", shaderFlagsAndAttributes.shaderFlags);
    shaderCode.vertexShaderCode = replaceString(pbrNormalBaseVertexCode, "#INSERT_VERTEX_ATTRIBUTES", shaderFlagsAndAttributes.shaderVertexAttributes);

    shaderCode.fragmentShaderCode = replaceString(pbrNormalBaseFragmentCode,"#INSERT_FLAGS", shaderFlagsAndAttributes.shaderFlags);

    return shaderCode;

}
