#include <unordered_map>
#include "rendering_system_data_types.h"
#include <map>








/*
* MAX TEXCOORDS = 5
* MAX COLOR SETS = 1
* MAX SKINNING JOINTS SETS = 1
* MAX SKINNING WEIGHTS SETS = 1
* 

* 
* ----ASSUMPTIONS----
* POSITION, NORMAL, TANGENT are always VEC3 FLOAT
* TEXCOORDS can be VEC2, VEC3, VEC4 with any ComponentDataType
* COLOR SET 0 can be VEC2, VEC3, VEC4 with any ComponentDataType
* JOINTS are always VEC4 with UNSIGNED_BYTE or UNSIGNED_SHORT, though format allows FLOAT dont use it
* WEIGHTS are always VEC4 with FLOAT or UNSIGNED_BYTE or UNSIGNED_SHORT
* 
* 
* -----BIT ALLOCATION EXPLANATION----
* 
* ComponentDataType : FLOAT(00), UNSIGNED_BYTE(01), UNSIGNED_SHORT(10)
* ComponentType : VEC2(00), VEC3(01), VEC4(10)
* 
* 
* 
|<-------------------------------------------PRESENCE BITS------------------------- ------------------>|<-------TEXCOORD COUNT & DATA TYPES----------------->|<-----COLOR INFO---------|---------DATA TYPE------------>|<--------------------------SKINNING INFO--------------------->|
|Position(1 bit)|Normal(1 bit)|Tangent(1 bit)|TexCoord(1 bit)|Color(1 bit)|Joints(1 bit)|Weights(1 bit)|  TexCount(3 bits)     |TexCoordDataType(2 bits each)|ColorComponentType(2 bit)|ColorComponentDataType(2 bits) |JointComponentDataType(2 bits)|WeightComponentDataType(2 bits)|
|<---------------Fixed types---------------->|<-------------------Variable types---------------------->|(1 to 5 ,0 means none) |   (2 * 5 =10 bits)          |                                                         |                              |                               |
0				1             2              3               4            5             6              7                       10                            20                        22                              24                             26                              28           

*/



namespace VertexFormatMask
{

    using MaskType = uint32_t;

    // --- 1. PRESENCE BITS (1 bit each, 0 to 6) ---

    // These are simple flags (powers of 2)
    constexpr MaskType MASK_PRESENCE_POSITION = 1U << 0;
    constexpr MaskType MASK_PRESENCE_NORMAL = 1U << 1;
    constexpr MaskType MASK_PRESENCE_TANGENT = 1U << 2;

    constexpr MaskType MASK_PRESENCE_TEXCOORD = 1U << 3;

    constexpr MaskType MASK_PRESENCE_COLOR = 1U << 4;
    constexpr MaskType MASK_PRESENCE_JOINTS = 1U << 5;
    constexpr MaskType MASK_PRESENCE_WEIGHTS = 1U << 6;


    // --- 2. TEXCOORD COUNT FIELD (3 bits, 7 to 9) ---

    constexpr uint32_t SHIFT_TEXCOORD_COUNT = 7;
    // 0b111 << 7 = 0b1110000000
    constexpr MaskType MASK_TEXCOORD_COUNT = 0x7U << SHIFT_TEXCOORD_COUNT;

    // --- 3. TEXCOORD DATA TYPE FIELDS (5 fields, 2 bits each, 10 to 19) ---

    // Base shift for the first set (TexCoord 0)
    constexpr uint32_t SHIFT_TEXCOORD_DATATYPE_BASE = 10;
    // Stride for subsequent sets
    constexpr uint32_t TEXCOORD_DATATYPE_STRIDE = 2;
    // 0b11
    constexpr MaskType MASK_TEXCOORD_DATATYPE_PER_SET = 0x3U;

    /**
     * @brief Calculates the shift for a specific TexCoord set.
     * @param index The set index (0 to 4).
     * @return The bit position for the field.
     */
    constexpr uint32_t getTexCoordShift(uint32_t index)
    {
        return SHIFT_TEXCOORD_DATATYPE_BASE + (index * TEXCOORD_DATATYPE_STRIDE);
    }

    // --- 4. COLOR, JOINTS, WEIGHTS FIELDS (2 bits each, 20 to 27) ---

    // Color Component Type (VEC2/VEC3/VEC4)
    constexpr uint32_t SHIFT_COLOR_COMPONENT_TYPE = 20;
    constexpr MaskType MASK_COLOR_COMPONENT_TYPE = 0x3U << SHIFT_COLOR_COMPONENT_TYPE;

    // Color Data Type (FLOAT/UBYTE/USHORT)
    constexpr uint32_t SHIFT_COLOR_DATATYPE = 22;
    constexpr MaskType MASK_COLOR_DATATYPE = 0x3U << SHIFT_COLOR_DATATYPE;

    // Joints Data Type (UBYTE/USHORT - FLOAT discouraged)
    constexpr uint32_t SHIFT_JOINTS_DATATYPE = 24;
    constexpr MaskType MASK_JOINTS_DATATYPE = 0x3U << SHIFT_JOINTS_DATATYPE;

    // Weights Data Type (FLOAT/UBYTE/USHORT)
    constexpr uint32_t SHIFT_WEIGHTS_DATATYPE = 26;
    constexpr MaskType MASK_WEIGHTS_DATATYPE = 0x3U << SHIFT_WEIGHTS_DATATYPE;

}











class VertexFormatHelper
{

private:

    // These constants make the map definition clearer
   static const size_t SIZE_FLOAT = 4;
   static const size_t SIZE_INT = 4;
   static const size_t SIZE_UNSIGNED_INT = 4;
   static const size_t SIZE_UNSIGNED_SHORT = 2;
   static const size_t SIZE_UNSIGNED_BYTE = 1;
   static const size_t SIZE_BYTE = 1;



   const static std::map <ComponentType, int> COMPONENT_TYPE_TO_NUM_COMPONENTS_MAP;
   const static std::map <ComponentDataType, int> COMPONENT_DATA_TYPE_TO_SIZE_IN_BYTES_MAP;




   //Format specific
   const static std::unordered_map<uint8_t,ComponentDataType > VERTEXFORMAT_COMPONENT_DATA_TYPE_TO_ENGINE_DATA_TYPE_MAP;
   const static std::unordered_map<uint8_t, ComponentType > VERTEXFORMAT_COMPONENT_TYPE_TO_ENGINE_COMPONENT_TYPE_MAP;

   const static std::unordered_map<ComponentDataType, uint8_t > ENGINE_DATA_TYPE_TO_VERTEXFORMAT_COMPONENT_DATA_TYPE_MAP;
   const static std::unordered_map<ComponentType, uint8_t > ENGINE_COMPONENT_TYPE_TO_VERTEXFORMAT_COMPONENT_TYPE_MAP;

   static ComponentDataType getComponentDataTypeFromBits(const uint8_t componentDataTypeBits);
   static ComponentType getComponentTypeFromBits(const uint8_t componentTypeBits);


public:

    static size_t getNumComponentsForComponentType(const ComponentType componentType);
    static size_t getSizeInBytesForComponentDataType(const ComponentDataType componentDataType);

	static uint8_t getBitsFromComponentDataType(const ComponentDataType componentDataType);
	static uint8_t getBitsFromComponentType(const ComponentType componentType);



    static std::vector<VertexAttributeInfo> getVertexAttributeInfosForVertexFormat(const VertexFormat vertexFormat);

   
    static size_t getSizeOfIndexTypeInBytes(const IndexType indexType);
    static size_t getSizeOfVertexForFormatInBytes(const VertexFormat vertexFormat);
 
   

	static VertexFormat createVertexFormatFromAttributeInfos(const std::vector<VertexAttributeInfo>& attributeInfos);


};