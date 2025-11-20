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
   static const std::unordered_map<VertexAttributeType, size_t> m_attributeSizeMap; 


public:

   static size_t getSizeInBytesOfSingleAttribute(VertexAttributeType vertexAttributeType);
   static size_t getSizeOfVertexForFormatInBytes(VertexFormat vertexFormat);

   static VertexAttribute GetVertexAttributeDetails(VertexAttributeType type);
    
};