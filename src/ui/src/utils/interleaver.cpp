#include "../../../../include/ui/include/utils/interleaver.h"
#include <cassert>



std::vector<std::byte> Interleaver::interleave(const std::vector<std::vector<std::byte>>& srcDatas, const std::vector<size_t>& bytesPerSetOfData)
{

	if (srcDatas.size() != bytesPerSetOfData.size())
	{
		assert(0);
		return std::vector<std::byte>();

	}
	//Getting total size needed and validation
	size_t totalByteSize = 0;
	size_t numOfComponents = 0;
	size_t stride = 0;
	for(int i = 0; i < srcDatas.size(); ++i)
	{
		const auto& dataSet = srcDatas[i];
		const size_t bytesPerComponent = bytesPerSetOfData[i];

		stride += bytesPerComponent;

		const size_t numOfBytes = dataSet.size();


		const size_t tempNumOfComponents = numOfBytes / bytesPerComponent;

		if (i != 0 && tempNumOfComponents != numOfComponents)
		{
			assert(0);
			return std::vector<std::byte>();
		}

		numOfComponents = tempNumOfComponents;
		totalByteSize += numOfBytes;


		

	}


	std::vector<std::byte> interleavedData(totalByteSize, static_cast<std::byte>(0));
	

	const size_t COUNT = srcDatas.size();
	size_t currentOffset = 0;
	const size_t NUM_OF_ELEMENTS = numOfComponents;
	const size_t STRIDE = stride;

	for (size_t i = 0; i < COUNT; ++i)
	{
		//assuming ordered
		//const VertexAttributeInfo& attributeInfo = vertexAttributeInfos[i];
		const std::vector<std::byte>& srcRawData = srcDatas[i];



		const size_t offset = currentOffset;
		const size_t attributeSizeInBytes = bytesPerSetOfData[i];


		std::byte* destBasePtr = interleavedData.data();
		const std::byte* srcBasePtr = srcRawData.data();

		for (size_t i = 0; i < NUM_OF_ELEMENTS; ++i)
		{



			std::byte* finalDestPtr = destBasePtr + (offset + i * STRIDE);
			const std::byte* finalSrcPtr = srcBasePtr + (i * attributeSizeInBytes);

			memcpy(finalDestPtr, finalSrcPtr, attributeSizeInBytes);
		}
		currentOffset += attributeSizeInBytes;

	}





	return interleavedData;
}