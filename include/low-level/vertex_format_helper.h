#include <unordered_map>
#include "rendering_system_data_types.h"


class VertexFormatHelper
{

private:
    // These constants make the map definition clearer
   static const size_t SIZE_FLOAT = 4;
   static const size_t SIZE_INT = 4;
   static const size_t SIZE_UNSIGNED_BYTE = 1;


    // C++11 guarantees safe concurrent initialization of function static variables
    static const std::unordered_map<VertexAttributeType, size_t> m_attributeSizeMap =
    {
        // Attribute Type, Size in Bytes

        // 3 floats = 12 bytes
        {VertexAttributeType::POSITION_3F, SIZE_FLOAT * 3},
        {VertexAttributeType::NORMAL_3F, SIZE_FLOAT * 3},
        {VertexAttributeType::TANGENT_3F, SIZE_FLOAT * 3},
        {VertexAttributeType::BINORMAL_3F, SIZE_FLOAT * 3},

        // Texture and Color Data
        {VertexAttributeType::TEXCOORD_0_2F, SIZE_FLOAT * 2}, // 2 floats = 8 bytes
        {VertexAttributeType::COLOR_0_4UB_NORMALIZED, SIZE_UNSIGNED_BYTE * 4}, // 4 bytes

        // Auxiliary Data
        {VertexAttributeType::JOINT_INDICES_4I, SIZE_INT * 4}, // 4 integers = 16 bytes
        {VertexAttributeType::JOINT_WEIGHTS_4F, SIZE_FLOAT * 4}, // 4 floats = 16 bytes

        {VertexAttributeType::UNKNOWN, 0}
    };


public:

   static size_t getSizeInBytesOfSingleAttribute(VertexAttributeType vertexAttributeType);
   static size_t getSizeOfVertexForFormatInBytes(VertexFormat vertexFormat);

   static VertexAttribute GetVertexAttributeDetails(VertexAttributeType type);
    
};