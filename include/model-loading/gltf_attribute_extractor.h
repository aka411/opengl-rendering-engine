#include <string>
#include "../low-level/rendering_system_data_types.h"
#include <vector>
#include "tiny_gltf.h"

struct AttributeExtractionResult
{
	int accessorIndex;
	VertexAttributeInfo vertexAttributeInfo;
};

class GTLFAttributeExtractor
{

private:

	//gltf uses string for attribute name and tinygltf uses string for attribute name
	const static std::map<std::string, VertexAttributeType> BASE_ATTRIBUTE_MAP;

	//gltf uses string for accessor type,but tinygltf uses int for componentType
	//const static std::map<std::string, ComponentType> GLTF_COMPONENT_TYPE_MAP;
	
	//for parsing tinygltf accessor type,
	const static std::map<int, ComponentType> GLTF_COMPONENT_TYPE_MAP;



	//tinygltf and gltf use int for componentType
	const static std::map<int, ComponentDataType> GLTF_COMPONENT_DATA_TYPE_MAP;


public:


	static bool getGltfAttributeTypeFromGltf(const std::string& gltfName, VertexAttributeInfo& info);

	static ComponentType getComponentTypeFromGLTFAccessorType(const int gltfComponentType);
	static ComponentDataType getComponentDataTypeFromGLTFAccessorComponentType(const int gltfComponentDataType);


	//sorted by our engine attribute order
	static std::vector<AttributeExtractionResult> getEngineAttributesToAccessorIndex(const tinygltf::Primitive& gltfPrimitive, const std::vector<tinygltf::Accessor>& gltfAccessor);


};