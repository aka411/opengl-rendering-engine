#include <cassert>
#include <cstddef>
#include <cstring>


#include "../../include/model-loading/vertex_attributes_repacker.h"
#include "../../include/low-level/vertex_format_helper.h"




VertexData VertexAttributeRepacker::interleaveVertexAttributes(const ExtractedAttributeData& extractedAttributeData)
{

	const std::vector<VertexAttributeInfo>& vertexAttributeInfos = extractedAttributeData.vertexAttributeInfos;
	const std::vector<std::vector<std::byte>>& vertexAttributeRawDatas = extractedAttributeData.vertexAttributeRawDatas;




    size_t totalByteSize = 0;
    size_t numVertices = 0;
    size_t stride = 0;

	assert(vertexAttributeInfos.size() == vertexAttributeRawDatas.size() && "Attribute infos and raw data size mismatch!");

	const size_t ATTRIBUTE_COUNT = vertexAttributeInfos.size();


    for (size_t i = 0 ; i < ATTRIBUTE_COUNT; ++i)
    {
		const VertexAttributeInfo& attributeInfo = vertexAttributeInfos[i];
		const std::vector<std::byte>& rawData = vertexAttributeRawDatas[i];

		const size_t attributeSize = attributeInfo.sizeInBytes;

        stride += attributeSize;
        
        totalByteSize += rawData.size();
		assert(rawData.size() % attributeSize == 0 && "Raw data size must be multiple of attribute size!");
        const size_t attributeTotalNumOfElements = rawData.size() / attributeSize;
        if (numVertices == 0)
        {
            numVertices = attributeTotalNumOfElements;
        }
        else
        {
            // Add an assert/exception check
            assert(numVertices == attributeTotalNumOfElements && "All vertex attribute arrays must have the same number of vertices!");
        }



    }


    VertexData vertexData;



    


    vertexData.vertexLayout = VertexLayout::INTERLEAVED;
    vertexData.vertexCount = numVertices;
    vertexData.vertexFormat = extractedAttributeData.vertexFormat;
   
    std::vector<std::byte>& interleavedData = vertexData.data;
    interleavedData.resize(totalByteSize);


    size_t currentOffset = 0;
    for (size_t i = 0; i < ATTRIBUTE_COUNT; ++i)
    {
        //assuming ordered
        const VertexAttributeInfo& attributeInfo = vertexAttributeInfos[i];
        const std::vector<std::byte>& srcRawData = vertexAttributeRawDatas[i];

       

        const size_t offset = currentOffset;
        const size_t attributeSizeInBytes = attributeInfo.sizeInBytes;


        std::byte* destBasePtr = interleavedData.data();
        const std::byte* srcBasePtr = srcRawData.data();

        for (size_t i = 0; i < numVertices; ++i)
        {



            std::byte* finalDestPtr = destBasePtr + (offset + i * stride);
            const std::byte* finalSrcPtr = srcBasePtr + (i * attributeSizeInBytes);

            memcpy(finalDestPtr, finalSrcPtr, attributeSizeInBytes);
        }
		currentOffset += attributeSizeInBytes;

    }



	return vertexData;
}
