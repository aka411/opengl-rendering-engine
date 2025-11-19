#include "../../include/low-level/vertex_format_helper.h"
#include <algorithm>
#include <vector>
#include "../../include/low-level/rendering_system_data_types.h"





size_t VertexFormatHelper::getSizeInBytesOfSingleAttribute(VertexAttributeType vertexAttributeType)
{


	return m_attributeSizeMap.at(vertexAttributeType);


}


size_t VertexFormatHelper::getSizeOfVertexForFormatInBytes(VertexFormat vertexFormat)
{

	size_t totalSize = 0;
	for (int i = 0; i < vertexFormat.size(); ++i)
	{
		if (vertexFormat.test(i))
		{
			totalSize += m_attributeSizeMap.at(static_cast<VertexAttributeType>(i));
		}
	}

	return totalSize;

}



VertexAttribute VertexFormatHelper::GetVertexAttributeDetails(VertexAttributeType type)
{
	VertexAttribute attr = { type, 0, 0, 0, GL_FALSE, 0 };

	switch (type)
	{
	case VertexAttributeType::POSITION_3F:
	case VertexAttributeType::NORMAL_3F:
	case VertexAttributeType::TANGENT_3F:
	case VertexAttributeType::BINORMAL_3F:
		attr.count = 3;
		attr.glType = GL_FLOAT;
		attr.size = 3 * sizeof(float);
		break;

	case VertexAttributeType::TEXCOORD_0_2F:
	case VertexAttributeType::TEXCOORD_1_2F:
	case VertexAttributeType::TEXCOORD_2_2F:
	case VertexAttributeType::TEXCOORD_3_2F:
		attr.count = 2;
		attr.glType = GL_FLOAT;
		attr.size = 2 * sizeof(float);
		break;

	case VertexAttributeType::COLOR_0_4UB_NORMALIZED:
	case VertexAttributeType::COLOR_1_4UB_NORMALIZED:
	case VertexAttributeType::COLOR_2_4UB_NORMALIZED:
	case VertexAttributeType::COLOR_3_4UB_NORMALIZED:

		attr.count = 4;
		attr.glType = GL_UNSIGNED_BYTE;
		attr.normalized = GL_TRUE; // This is the key difference for normalized types
		attr.size = 4 * sizeof(unsigned char);
		break;

	case VertexAttributeType::JOINT_INDICES_4I:
		attr.count = 4;
		attr.glType = GL_INT;
		attr.size = 4 * sizeof(int);
		break;

	case VertexAttributeType::JOINT_WEIGHTS_4F:
		attr.count = 4;
		attr.glType = GL_FLOAT;
		attr.size = 4 * sizeof(float);
		break;

	default:
		// Handle UNKNOWN case
		break;
	}

	return attr;
}
