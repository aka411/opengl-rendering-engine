#include "../../include/low-level/vertex_format_helper.h"
#include <algorithm>
#include <vector>
#include "../../include/low-level/rendering_system_data_types.h"
#include <cassert>



const std::unordered_map<uint8_t, ComponentDataType > VertexFormatHelper::VERTEXFORMAT_COMPONENT_DATA_TYPE_TO_ENGINE_DATA_TYPE_MAP =
{


	{0b00,ComponentDataType::FLOAT},
	{0b01,ComponentDataType::UNSIGNED_BYTE},
	{0b10,ComponentDataType::UNSIGNED_SHORT}
	//0b11 is unused/reserved

};


const  std::unordered_map<uint8_t, ComponentType > VertexFormatHelper::VERTEXFORMAT_COMPONENT_TYPE_TO_ENGINE_COMPONENT_TYPE_MAP =
{
	{0b00,ComponentType::VEC2},
	{0b01,ComponentType::VEC3},
	{0b10,ComponentType::VEC4}
	//0b11 is unused/reserved

};




const std::unordered_map<ComponentDataType, uint8_t > VertexFormatHelper::ENGINE_DATA_TYPE_TO_VERTEXFORMAT_COMPONENT_DATA_TYPE_MAP =
{
	{ComponentDataType::FLOAT,0b00},
	{ComponentDataType::UNSIGNED_BYTE,0b01},
	{ComponentDataType::UNSIGNED_SHORT,0b10}
	//0b11 is unused/reserved
};

const std::unordered_map<ComponentType, uint8_t > VertexFormatHelper::ENGINE_COMPONENT_TYPE_TO_VERTEXFORMAT_COMPONENT_TYPE_MAP =
{
	{ComponentType::VEC2,0b00},
	{ComponentType::VEC3,0b01},
	{ComponentType::VEC4,0b10}
	//0b11 is unused/reserved
};







const std::map <ComponentType, int> VertexFormatHelper::COMPONENT_TYPE_TO_NUM_COMPONENTS_MAP =
{
	{ComponentType::SCALAR,1},
	{ComponentType::VEC2,2},
	{ComponentType::VEC3,3},
	{ComponentType::VEC4,4}
	//others for future use
	//{ComponentType::MAT2,4},
	//{ComponentType::MAT3,9},
	//{ComponentType::MAT4,16}
};


const std::map <ComponentDataType, int> VertexFormatHelper::COMPONENT_DATA_TYPE_TO_SIZE_IN_BYTES_MAP =
{

	{ComponentDataType::UNSIGNED_BYTE,1},
	{ComponentDataType::UNSIGNED_SHORT,2},
	{ComponentDataType::FLOAT,4 }
	//others for future use
	//{ComponentDataType::BYTE,1},
	//{ComponentDataType::SHORT,2},
	//{ComponentDataType::UNSIGNED_INT,4}
};








size_t VertexFormatHelper::getNumComponentsForComponentType(const ComponentType componentType)
{

	const auto& it = COMPONENT_TYPE_TO_NUM_COMPONENTS_MAP.find(componentType);

	if (it != COMPONENT_TYPE_TO_NUM_COMPONENTS_MAP.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}


}

size_t VertexFormatHelper::getSizeInBytesForComponentDataType(const ComponentDataType componentDataType)
{
	const auto& it = COMPONENT_DATA_TYPE_TO_SIZE_IN_BYTES_MAP.find(componentDataType);
	if (it != COMPONENT_DATA_TYPE_TO_SIZE_IN_BYTES_MAP.end())
	{
		return it->second;
	}
	else
	{
		return 0;
	}
}



 uint8_t VertexFormatHelper::getBitsFromComponentDataType(const ComponentDataType componentDataType)
{
	 const auto& it = VertexFormatHelper::ENGINE_DATA_TYPE_TO_VERTEXFORMAT_COMPONENT_DATA_TYPE_MAP.find(componentDataType);

	if (it != VertexFormatHelper::ENGINE_DATA_TYPE_TO_VERTEXFORMAT_COMPONENT_DATA_TYPE_MAP.end())
	{
		return it->second;
	}
	else
	{
		return 0b11; //reserved/invalid
	}
}

uint8_t VertexFormatHelper::getBitsFromComponentType(const ComponentType componentType)
{
	const auto& it = VertexFormatHelper::ENGINE_COMPONENT_TYPE_TO_VERTEXFORMAT_COMPONENT_TYPE_MAP.find(componentType);
	if (it != VertexFormatHelper::ENGINE_COMPONENT_TYPE_TO_VERTEXFORMAT_COMPONENT_TYPE_MAP.end())
	{
		return it->second;
	}
	else
	{
		return 0b11; //reserved/invalid
	}
}


ComponentDataType VertexFormatHelper::getComponentDataTypeFromBits(const uint8_t componentDataTypeBits)
{
	const auto& it = VERTEXFORMAT_COMPONENT_DATA_TYPE_TO_ENGINE_DATA_TYPE_MAP.find(componentDataTypeBits);


	if (it != VERTEXFORMAT_COMPONENT_DATA_TYPE_TO_ENGINE_DATA_TYPE_MAP.end())
	{
		return it->second;
	}
	else
	{
		return ComponentDataType::UNKNOWN;
	}


}








ComponentType VertexFormatHelper::getComponentTypeFromBits(const uint8_t componentTypeBits)
{

	const auto& it = VERTEXFORMAT_COMPONENT_TYPE_TO_ENGINE_COMPONENT_TYPE_MAP.find(componentTypeBits);


	if (it != VERTEXFORMAT_COMPONENT_TYPE_TO_ENGINE_COMPONENT_TYPE_MAP.end())
	{
		return it->second;
	}
	else
	{
		return ComponentType::UNKNOWN;
	}

}








std::vector<VertexAttributeInfo> VertexFormatHelper::getVertexAttributeInfosForVertexFormat(const VertexFormat vertexFormat)
{
	std::vector<VertexAttributeInfo> attributeInfos;



	// Check for POSITION
	if (vertexFormat & VertexFormatMask::MASK_PRESENCE_POSITION)
	{
		VertexAttributeInfo positionInfo;
		positionInfo.vertexAttributeType = VertexAttributeType::POSITION;
		positionInfo.componentType = ComponentType::VEC3;
		positionInfo.componentDataType = ComponentDataType::FLOAT;
		positionInfo.sizeInBytes = 3 * 4; // VEC3 * FLOAT
		attributeInfos.push_back(positionInfo);
	}
	if( vertexFormat & VertexFormatMask::MASK_PRESENCE_NORMAL)
	{
		VertexAttributeInfo normalInfo;
		normalInfo.vertexAttributeType = VertexAttributeType::NORMAL;
		normalInfo.componentType = ComponentType::VEC3;
		normalInfo.componentDataType = ComponentDataType::FLOAT;
		normalInfo.sizeInBytes = 3 * 4; // VEC3 * FLOAT
		attributeInfos.push_back(normalInfo);
	}
	if( vertexFormat & VertexFormatMask::MASK_PRESENCE_TANGENT)
	{
		VertexAttributeInfo tangentInfo;
		tangentInfo.vertexAttributeType = VertexAttributeType::TANGENT;
		tangentInfo.componentType = ComponentType::VEC4; // As per GLTF spec w component is for handiness
		tangentInfo.componentDataType = ComponentDataType::FLOAT;
		tangentInfo.sizeInBytes = 4 * 4; // VEC3 * FLOAT
		attributeInfos.push_back(tangentInfo);
	}

	// Check for TEXCOORD

	if( vertexFormat & VertexFormatMask::MASK_PRESENCE_TEXCOORD)
	{
		// Extract TexCoord count
		uint32_t texCoordCount = (vertexFormat & VertexFormatMask::MASK_TEXCOORD_COUNT) >> VertexFormatMask::SHIFT_TEXCOORD_COUNT;
		for(uint32_t i = 0; i < texCoordCount; ++i)
		{
			VertexAttributeInfo texCoordInfo;
			texCoordInfo.vertexAttributeType = VertexAttributeType::TEXCOORD;
			texCoordInfo.index = static_cast<int>(i);

			// Extract ComponentDataType for this TexCoord
			uint32_t shift = VertexFormatMask::getTexCoordShift(i);
			uint32_t texCoordDataTypeBits = static_cast<uint32_t>(((vertexFormat) >> shift)& VertexFormatMask::MASK_TEXCOORD_DATATYPE_PER_SET);

			texCoordInfo.componentDataType = getComponentDataTypeFromBits(texCoordDataTypeBits);
			//  TEXCOORDs are VEC2 and always VEC2. This assumption is based on GLTF specs.
			texCoordInfo.componentType = ComponentType::VEC2;

			// Calculate size in bytes
			size_t componentDataTypeSize = getSizeInBytesForComponentDataType(texCoordInfo.componentDataType);

			const size_t NUM_OF_COMPONENTS_IN_VEC2 = 2; // VEC2
			texCoordInfo.sizeInBytes = NUM_OF_COMPONENTS_IN_VEC2 * componentDataTypeSize; // VEC2
			attributeInfos.push_back(texCoordInfo);
		}
	}


	if (vertexFormat & VertexFormatMask::MASK_PRESENCE_COLOR)
	{


		VertexAttributeInfo colorInfo;
		colorInfo.vertexAttributeType = VertexAttributeType::COLOR;

		uint32_t colorComponentTypeBits = static_cast<uint32_t>(vertexFormat & VertexFormatMask::MASK_COLOR_COMPONENT_TYPE) >> VertexFormatMask::SHIFT_COLOR_COMPONENT_TYPE;
		uint32_t colorComponentDataTypeBits = static_cast<uint32_t>(vertexFormat & VertexFormatMask::MASK_COLOR_DATATYPE) >> VertexFormatMask::SHIFT_COLOR_DATATYPE;

		colorInfo.componentType = getComponentTypeFromBits(colorComponentTypeBits);
		colorInfo.componentDataType = getComponentDataTypeFromBits(colorComponentDataTypeBits);
		colorInfo.index = 0; //Single color attribute
		const size_t NUM_OF_COMPONENT_FOR_COMPONENT_TYPE =  getNumComponentsForComponentType(colorInfo.componentType);
		const size_t NUM_OF_BYTES_COMPONENT_DATA_TYPE = getSizeInBytesForComponentDataType(colorInfo.componentDataType);

		colorInfo.sizeInBytes = NUM_OF_BYTES_COMPONENT_DATA_TYPE * NUM_OF_COMPONENT_FOR_COMPONENT_TYPE;


		attributeInfos.push_back(colorInfo);


	}



	if (vertexFormat & VertexFormatMask::MASK_PRESENCE_JOINTS)
	{

		VertexAttributeInfo jointInfo;
		jointInfo.vertexAttributeType = VertexAttributeType::JOINTS;


		uint32_t jointComponentDataTypeBits = static_cast<uint32_t>(vertexFormat & VertexFormatMask::MASK_JOINTS_DATATYPE) >> VertexFormatMask::SHIFT_JOINTS_DATATYPE;

		jointInfo.componentType = ComponentType::VEC4;
		jointInfo.componentDataType = getComponentDataTypeFromBits(jointComponentDataTypeBits);

		const size_t NUM_OF_COMPONENT_FOR_COMPONENT_TYPE = getNumComponentsForComponentType(jointInfo.componentType);
		const size_t NUM_OF_BYTES_COMPONENT_DATA_TYPE = getSizeInBytesForComponentDataType(jointInfo.componentDataType);

		jointInfo.sizeInBytes = NUM_OF_BYTES_COMPONENT_DATA_TYPE * NUM_OF_COMPONENT_FOR_COMPONENT_TYPE;


		attributeInfos.push_back(jointInfo);
	}

	if (vertexFormat & VertexFormatMask::MASK_PRESENCE_WEIGHTS)
	{

			VertexAttributeInfo weightInfo;
			weightInfo.vertexAttributeType = VertexAttributeType::WEIGHTS;


			uint32_t weightComponentDataTypeBits = static_cast<uint32_t>(vertexFormat & VertexFormatMask::MASK_WEIGHTS_DATATYPE) >> VertexFormatMask::SHIFT_WEIGHTS_DATATYPE;

			weightInfo.componentType = ComponentType::VEC4;
			weightInfo.componentDataType = getComponentDataTypeFromBits(weightComponentDataTypeBits);

			const size_t NUM_OF_COMPONENT_FOR_COMPONENT_TYPE = getNumComponentsForComponentType(weightInfo.componentType);
			const size_t NUM_OF_BYTES_COMPONENT_DATA_TYPE = getSizeInBytesForComponentDataType(weightInfo.componentDataType);

			weightInfo.sizeInBytes = NUM_OF_BYTES_COMPONENT_DATA_TYPE * NUM_OF_COMPONENT_FOR_COMPONENT_TYPE;


			attributeInfos.push_back(weightInfo);
		
	}







	return attributeInfos;
}



size_t VertexFormatHelper::getSizeOfIndexTypeInBytes(const IndexType indexType)
{

	switch (indexType)
	{
	case IndexType::UBYTE_8: return 1;
	case IndexType::USHORT_16: return 2;
	case IndexType::UINT_32: return 4;
	
	default: return 0;



	}


}




size_t VertexFormatHelper::getSizeOfVertexForFormatInBytes(const VertexFormat vertexFormat)
{

	size_t totalSize = 0;

	std::vector<VertexAttributeInfo> vertexAttributeInfos = getVertexAttributeInfosForVertexFormat(vertexFormat);



	for(const VertexAttributeInfo& attrInfo : vertexAttributeInfos)
	{
		totalSize += attrInfo.sizeInBytes;
	}

	return totalSize;

}



VertexFormat VertexFormatHelper::createVertexFormatFromAttributeInfos(const std::vector<VertexAttributeInfo>& attributeInfos)
{
	VertexFormat vertexFormat = 0;

	for (const VertexAttributeInfo& attrInfo : attributeInfos)
	{
		const VertexAttributeType attributeType = attrInfo.vertexAttributeType;
		

		switch (attributeType)
		{
		case VertexAttributeType::POSITION:
		{


			vertexFormat = vertexFormat | VertexFormatMask::MASK_PRESENCE_POSITION;


		}break;
		case VertexAttributeType::NORMAL:
		{
			vertexFormat = vertexFormat | VertexFormatMask::MASK_PRESENCE_NORMAL;
		}break;

		case VertexAttributeType::TANGENT:
		{
			vertexFormat = vertexFormat | VertexFormatMask::MASK_PRESENCE_TANGENT;
		}break;

		case VertexAttributeType::TEXCOORD:
		{

			vertexFormat = vertexFormat | VertexFormatMask::MASK_PRESENCE_TEXCOORD;
			//Set TexCoord count
			uint32_t texCoordCount = ((vertexFormat & VertexFormatMask::MASK_TEXCOORD_COUNT) >> VertexFormatMask::SHIFT_TEXCOORD_COUNT) + 1;
			vertexFormat = (vertexFormat & ~VertexFormatMask::MASK_TEXCOORD_COUNT) | (texCoordCount << VertexFormatMask::SHIFT_TEXCOORD_COUNT);
			//Set TexCoord data type
			uint32_t shift = VertexFormatMask::getTexCoordShift(static_cast<uint32_t>(attrInfo.index));
			uint32_t componentDataTypeBits = getBitsFromComponentDataType(attrInfo.componentDataType);



			vertexFormat = vertexFormat | (componentDataTypeBits << shift);
		}break;

		case VertexAttributeType::COLOR:
		{
			vertexFormat = vertexFormat | VertexFormatMask::MASK_PRESENCE_COLOR;
			uint32_t colorComponentTypeBits = getBitsFromComponentType(attrInfo.componentType);
			uint32_t colorComponentDataTypeBits = getBitsFromComponentDataType(attrInfo.componentDataType);
			vertexFormat = vertexFormat | (colorComponentTypeBits << VertexFormatMask::SHIFT_COLOR_COMPONENT_TYPE);
			vertexFormat = vertexFormat | (colorComponentDataTypeBits << VertexFormatMask::SHIFT_COLOR_DATATYPE);

		}break;
		case VertexAttributeType::JOINTS:
		{
			vertexFormat = vertexFormat | VertexFormatMask::MASK_PRESENCE_JOINTS;
			uint32_t jointComponentDataTypeBits = getBitsFromComponentDataType(attrInfo.componentDataType);
			vertexFormat = vertexFormat | (jointComponentDataTypeBits << VertexFormatMask::SHIFT_JOINTS_DATATYPE);
		}
		break;
		case VertexAttributeType::WEIGHTS:
		{
			vertexFormat = vertexFormat | VertexFormatMask::MASK_PRESENCE_WEIGHTS;
			uint32_t weightComponentDataTypeBits = getBitsFromComponentDataType(attrInfo.componentDataType);
			vertexFormat = vertexFormat | (weightComponentDataTypeBits << VertexFormatMask::SHIFT_WEIGHTS_DATATYPE);
		}
		break;
		default:
			assert(false && "Unknown VertexAttributeType in createVertexFormatFromAttributeInfos");
			break;
		}


	}


	return vertexFormat;
}