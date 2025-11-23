#include <cassert>
#include <cstddef>
#include <cstring>


#include "../../include/model-loading/vertex_attributes_repacker.h"
#include "../../include/low-level/vertex_format_helper.h"




VertexData VertexAttributeRepacker::interleaveVertexAttributes(const std::map<VertexAttributeType, std::vector<std::byte>>& vertexAttributeToRawDatas)
{

    


    VertexData vertexData;
 
    size_t totalByteSize = 0;

    std::map<VertexAttributeType, size_t> attributeOffsets;
 
    size_t currentOffset = 0;
    size_t numVertices = 0;
    VertexFormat vertexFormat;
    for (const auto& vertexAttributeToRawData : vertexAttributeToRawDatas)
    {


        VertexAttributeType type = vertexAttributeToRawData.first;

        vertexFormat.set(static_cast<uint32_t>(type));

        const std::vector<std::byte>& rawData = vertexAttributeToRawData.second;
        size_t attributeSize = VertexFormatHelper::getSizeInBytesOfSingleAttribute(type); // Get size from your map

        // 1. Store the offset, keyed by the attribute type
        attributeOffsets[type] = currentOffset;

        // 2. Update the running offset (which is the Stride)
        currentOffset += attributeSize;

        // 3. Calculate total size (and number of vertices)
        totalByteSize += rawData.size();


        const size_t currentAttributeVertices = rawData.size() / attributeSize;
        if (numVertices == 0)
        {
            numVertices = currentAttributeVertices;
        }
        else
        {
            // Add an assert/exception check
            assert(numVertices == currentAttributeVertices && "All vertex attribute arrays must have the same number of vertices!");
        }

    }
    const size_t stride = currentOffset;


    vertexData.vertexLayout = VertexLayout::INTERLEAVED;
    vertexData.vertexCount = numVertices;
    vertexData.vertexFormat = vertexFormat;
   
    std::vector<std::byte>& interleavedData = vertexData.data;
    interleavedData.resize(totalByteSize);



    for (const auto& vertexAttributeToRawData : vertexAttributeToRawDatas)
    {

        const std::vector<std::byte>& srcRawByteData = vertexAttributeToRawData.second;

        const size_t offset = attributeOffsets.at(vertexAttributeToRawData.first);
        const size_t attributeSizeInBytes = VertexFormatHelper::getSizeInBytesOfSingleAttribute(vertexAttributeToRawData.first);

       // std::uintptr_t baseDestAddress = reinterpret_cast<std::uintptr_t>(interleavedData.data()) ;
        //std::uintptr_t baseSrcAddress = reinterpret_cast<std::uintptr_t>(srcRawByteData.data());

        std::byte* destBasePtr = interleavedData.data();
        const std::byte* srcBasePtr = srcRawByteData.data();

        for (size_t i = 0; i < numVertices; ++i)
        {

           // std::uintptr_t destByteIndex = offset + i * stride;
            ///std::uintptr_t srcByteIndex = i * attributeSizeInBytes;

           // void* finalDestPtr = reinterpret_cast<void*>(baseDestAddress + destByteIndex);
           // void* finalSrcPtr = reinterpret_cast<void*>(baseSrcAddress + srcByteIndex);

            std::byte* finalDestPtr = destBasePtr + (offset + i * stride);
            const std::byte* finalSrcPtr = srcBasePtr + (i * attributeSizeInBytes);

            memcpy(finalDestPtr, finalSrcPtr, attributeSizeInBytes);
        }

    }



	return vertexData;
}
