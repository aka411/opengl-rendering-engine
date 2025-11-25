#include <vector>
#include "../../include/model-loading/gltf_attribute_extractor.h"
#include <iostream>
#include  <algorithm>
#include "../../include/low-level/vertex_format_helper.h"

/***COMPONENT TYPE AND DATA TYPE METRICS *****/









const std::map<std::string, VertexAttributeType> GLTFAttributeExtractor::BASE_ATTRIBUTE_MAP = {
{"POSITION", VertexAttributeType::POSITION},
{"NORMAL", VertexAttributeType::NORMAL},
{"TANGENT", VertexAttributeType::TANGENT},
{"TEXCOORD", VertexAttributeType::TEXCOORD},
{"COLOR", VertexAttributeType::COLOR},
{"JOINTS", VertexAttributeType::JOINTS},
{"WEIGHTS", VertexAttributeType::WEIGHTS}
};

/*
const std::map<std::string, ComponentType> GTLFAttributeExtractor::GLTF_COMPONENT_TYPE_MAP =
{
    /*
        Allowed values :
    "SCALAR"
    "VEC2"
    "VEC3"
    "VEC4"
    "MAT2"
    "MAT3"
    "MAT4"//////
    


    {"SCALAR",ComponentType::SCALAR},
    {"VEC2",ComponentType::VEC2},
    {"VEC3",ComponentType::VEC3},
    {"VEC4",ComponentType::VEC4},

    //others for future use
    //{"MAT2",ComponentType::MAT2},
    //{"MAT3",ComponentType::MAT3},
    //{"MAT4",ComponentType::MAT4}

};
**/




//TinyGLTF uses int for componentType
const std::map<int, ComponentType> GLTFAttributeExtractor::GLTF_COMPONENT_TYPE_MAP =
{

    {1,ComponentType::SCALAR}, // SCALAR
    {2,ComponentType::VEC2},   // VEC2
    {3,ComponentType::VEC3},   // VEC3
	{4,ComponentType::VEC4}   // VEC4


};







const std::map<int, ComponentDataType> GLTFAttributeExtractor::GLTF_COMPONENT_DATA_TYPE_MAP =
{
    /*
        Allowed values :

    5120 BYTE

    5121 UNSIGNED_BYTE //whats the  is difference between BYTE AND UNSIGNED BYTE?

    5122 SHORT

    5123 UNSIGNED_SHORT

    5125 UNSIGNED_INT

    5126 FLOAT
    */

    //{5120,ComponentDataType::BYTE}, // BYTE
    {5121,ComponentDataType::UNSIGNED_BYTE},
    //{5122,ComponentDataType::SHORT}, // SHORT
    {5123,ComponentDataType::UNSIGNED_SHORT},
    //{5125,ComponentDataType::UNSIGNED_INT},

    {5126,ComponentDataType::FLOAT},

};










bool GLTFAttributeExtractor::getGltfAttributeTypeFromGltf(const std::string& gltfName, VertexAttributeInfo& info)
{
     
        size_t underscorePos = gltfName.find('_');

        std::string baseName;
        int index = -1; // Default index is -1

        if (underscorePos != std::string::npos)
        {
            // Name has an index suffix (e.g., "TEXCOORD_1")
            baseName = gltfName.substr(0, underscorePos);

            // Extracting the index part (e.g., "1") and convert to int
            std::string indexStr = gltfName.substr(underscorePos + 1);
            try
            {
                index = std::stoi(indexStr);
            }
            catch (...)
            {

                info.vertexAttributeType = VertexAttributeType::UNKNOWN;
                return false;
            }
        }
        else
        {
            // Name is a simple non-indexed type (e.g., "POSITION", "NORMAL")
            baseName = gltfName;
            index = -1; // Indicate no specific index or it's the primary (POSITION, NORMAL)
        }

        
        auto it = BASE_ATTRIBUTE_MAP.find(baseName);
        if (it != BASE_ATTRIBUTE_MAP.end())
        {
            info.vertexAttributeType = it->second;
            info.index = index;
            return true;
        }
        else
        {
            // Attribute name not recognized
           // std::cerr << "Error: Unknown attribute name: " << gltfName << std::endl;
            info.vertexAttributeType = VertexAttributeType::UNKNOWN;
            info.index = -1;
            return false;
        }

}

ComponentType GLTFAttributeExtractor::getComponentTypeFromGLTFAccessorType(const int gltfComponentType)
{

    auto it = GLTF_COMPONENT_TYPE_MAP.find(gltfComponentType);
    if (it != GLTF_COMPONENT_TYPE_MAP.end())
    {
        return it->second;
    }
    else
    {
        return ComponentType::UNKNOWN;
	}



}

ComponentDataType GLTFAttributeExtractor::getComponentDataTypeFromGLTFAccessorComponentType(const int gltfComponentDataType)
{

    auto it = GLTF_COMPONENT_DATA_TYPE_MAP.find(gltfComponentDataType);
    if (it != GLTF_COMPONENT_DATA_TYPE_MAP.end())
    {
        return it->second;
    }
    else
    {
        return ComponentDataType::UNKNOWN;
	}



 
}

std::vector<AttributeExtractionResult> GLTFAttributeExtractor::getEngineAttributesToAccessorIndex(const tinygltf::Primitive& gltfPrimitive, const std::vector<tinygltf::Accessor>& gltfAccessors)
{
	

	const std::map<std::string, int>& gltfAttributes = gltfPrimitive.attributes;

	std::vector<AttributeExtractionResult> results;
	results.reserve(gltfAttributes.size());

    for(const auto& attributePair : gltfAttributes)
    {
        const std::string& gltfAttributeName = attributePair.first;
        int accessorIndex = attributePair.second;
        VertexAttributeInfo attributeInfo;

        if (getGltfAttributeTypeFromGltf(gltfAttributeName, attributeInfo))
        {
            AttributeExtractionResult extractionResult;

            extractionResult.accessorIndex = accessorIndex;

			attributeInfo.componentDataType = getComponentDataTypeFromGLTFAccessorComponentType(gltfAccessors[accessorIndex].componentType);
			attributeInfo.componentType = getComponentTypeFromGLTFAccessorType(gltfAccessors[accessorIndex].type);
			attributeInfo.normalise = gltfAccessors[accessorIndex].normalized;

            const size_t numComponents = VertexFormatHelper::getNumComponentsForComponentType(attributeInfo.componentType);
            const size_t componentSizeInBytes = VertexFormatHelper::getSizeInBytesForComponentDataType(attributeInfo.componentDataType);

            attributeInfo.sizeInBytes = numComponents* componentSizeInBytes;
            extractionResult.vertexAttributeInfo = attributeInfo;



			assert(extractionResult.vertexAttributeInfo.vertexAttributeType != VertexAttributeType::UNKNOWN);
			assert(extractionResult.vertexAttributeInfo.componentDataType != ComponentDataType::UNKNOWN);
			assert(extractionResult.vertexAttributeInfo.componentType != ComponentType::UNKNOWN);
			assert(extractionResult.accessorIndex >= 0 && extractionResult.accessorIndex < static_cast<int>(gltfAccessors.size()));
			assert(attributeInfo.index < 5); // assuming a reasonable limit on attribute indices
            results.push_back(extractionResult);
        }
        else
        {
			std::cout << "Warning: Unknown attribute name in GLTF primitive: " << gltfAttributeName << std::endl;
			assert(false && "Unknown attribute name in GLTF primitive.");

        }
	}

    std::sort(results.begin(), results.end(), [](const auto& a , const auto& b)
        {
			//first try to sort by vertex attribute type
            if (a.vertexAttributeInfo.vertexAttributeType != b.vertexAttributeInfo.vertexAttributeType)
            {
                return static_cast<int>(a.vertexAttributeInfo.vertexAttributeType) < static_cast<int>(b.vertexAttributeInfo.vertexAttributeType);
            }
            else
            {
                //if same type , sort by index
                return a.vertexAttributeInfo.index < b.vertexAttributeInfo.index;
			}

        
        }
        
        );


	return results;

}