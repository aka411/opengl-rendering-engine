#include "../include/vertex_attribute_repacker.h"
#include <unordered_map>






//from accessor
//type
 const std::unordered_map<int, int> VertexAttributeRepacker::m_typeToNumOfComponentsMap = {
	 // Key (int type constant) : Value (number of components)
 { TINYGLTF_TYPE_SCALAR, 1 },
 { TINYGLTF_TYPE_VEC2,   2 },
 { TINYGLTF_TYPE_VEC3,   3 },
 { TINYGLTF_TYPE_VEC4,   4 },
 { TINYGLTF_TYPE_MAT2,   4 },  // 2x2 matrix
 { TINYGLTF_TYPE_MAT3,   9 },  // 3x3 matrix
 { TINYGLTF_TYPE_MAT4,   16 } // 4x4 matrix
};


//from accessor
 //componentType
 const std::unordered_map<int, int> VertexAttributeRepacker::m_componentTypeToByteSizeMap = {
	// Component ID : Bytes per component
	{5120, 1}, // signed byte (8 bits / 8)
	{5121, 1}, // unsigned byte (8 bits / 8)
	{5122, 2}, // signed short (16 bits / 8)
	{5123, 2}, // unsigned short (16 bits / 8)
	{5125, 4}, // unsigned int (32 bits / 8)
	{5126, 4}  // float (32 bits / 8)
};

 //Engine Format
 const std::unordered_map<Engine::VertexAttributes, int> VertexAttributeRepacker::m_attributeOffsetMap = {
	 //assuming 32 bit float values
	 //padding is an issue that currently is ignored if any problems occur might want to look here
	 {Engine::VertexAttributes::POSITION, 0},
	 {Engine::VertexAttributes::NORMAL, 12},
	 {Engine::VertexAttributes::TEXCOORD_0, 24},

	 {Engine::VertexAttributes::JOINT, 32},//  temp ivec4 4 * 4 bytes
	 {Engine::VertexAttributes::WEIGHT, 48}
	 //{Engine::VertexAttributes::WEIGHT, 36} //4 floats vec4



 };


BufferByteMetrics VertexAttributeRepacker::getBufferByteMetrics(tinygltf::Accessor& accessor, tinygltf::BufferView& bufferView)
{
	BufferByteMetrics bufferByteMetrics;

	bufferByteMetrics.absoluteBufferOffset = accessor.byteOffset + bufferView.byteOffset;
	bufferByteMetrics.numOfBytesPerItem = m_typeToNumOfComponentsMap.at(accessor.type) * m_componentTypeToByteSizeMap.at(accessor.componentType);
	
	bufferByteMetrics.numOfComponentsPerItem = m_typeToNumOfComponentsMap.at(accessor.type);
	bufferByteMetrics.totalBytes = accessor.count * bufferByteMetrics.numOfBytesPerItem;
	bufferByteMetrics.byteStride = (bufferView.byteStride > 0 )? bufferView.byteStride : 0 ;
	bufferByteMetrics.numOfBytesOfOneComponentOfItem = m_componentTypeToByteSizeMap.at(accessor.componentType);
	bufferByteMetrics.totalNumOfItems = accessor.count;
	return bufferByteMetrics;
}

ExtractedAttribute VertexAttributeRepacker::extractAttributeFromInterleaved(tinygltf::Buffer& buffer, const BufferByteMetrics& bufferByteMetrics)
{
	const size_t sizeOfBufferInbytes = buffer.data.size();
	//const unsigned char* byteBufferPtr = reinterpret_cast<const unsigned char*>(buffer.data);
 
	 ExtractedAttribute extractedAttribute;
	 extractedAttribute.totalNumOfItems = bufferByteMetrics.totalNumOfItems;
	 extractedAttribute.numOfBytesPerItem = bufferByteMetrics.numOfBytesPerItem;
	 extractedAttribute.numOfBytesOfOneComponentOfItem = bufferByteMetrics.numOfBytesOfOneComponentOfItem;

	 std::vector<std::byte> byteResult(bufferByteMetrics.totalBytes);

	
	 for (size_t i = 0; i < bufferByteMetrics.totalNumOfItems; ++i)
	 {
		 const size_t sourceIndex = bufferByteMetrics.absoluteBufferOffset + bufferByteMetrics.byteStride * i;
		 //safety check needed
		// std::copy_n(&buffer.data[sourceIndex], bufferByteMetrics.numOfBytePerComponent, &byteResult[i * bufferByteMetrics.numOfBytePerComponent]);
		 assert(0);
	 }
	 extractedAttribute.rawData = byteResult;
	return extractedAttribute;
}

ExtractedAttribute VertexAttributeRepacker::extractAttributeFromNonInterleaved(tinygltf::Buffer& buffer, const BufferByteMetrics& bufferByteMetrics)
{



	const size_t sizeOfBufferInbytes = buffer.data.size();
	//const unsigned char* byteBufferPtr = reinterpret_cast<const unsigned char*>(buffer.data);

	ExtractedAttribute extractedAttribute;
	extractedAttribute.totalNumOfItems = bufferByteMetrics.totalNumOfItems;
	extractedAttribute.numOfBytesPerItem = bufferByteMetrics.numOfBytesPerItem;
	extractedAttribute.numOfBytesOfOneComponentOfItem = bufferByteMetrics.numOfBytesOfOneComponentOfItem;

	if (extractedAttribute.numOfBytesOfOneComponentOfItem <= 4)
	{
		extractedAttribute.numOfBytesOfOneComponentOfItem = 4;
		extractedAttribute.numOfBytesPerItem = bufferByteMetrics.numOfComponentsPerItem * 4;
	}
	const size_t neededBufferSize = extractedAttribute.totalNumOfItems * extractedAttribute.numOfBytesPerItem;
	std::vector<std::byte> byteResult(neededBufferSize, std::byte(0));





	if (bufferByteMetrics.numOfBytesOfOneComponentOfItem >=4)
	{
		const size_t sourceIndex = bufferByteMetrics.absoluteBufferOffset;
		memcpy(&byteResult[0], &buffer.data[sourceIndex], bufferByteMetrics.totalBytes);//will it also copy padding?
	}
	else
	{
		for(size_t i = 0 ;i < bufferByteMetrics.totalNumOfItems; i+= bufferByteMetrics.numOfBytesOfOneComponentOfItem)
		{
		
			for (size_t j = 0; j < bufferByteMetrics.numOfComponentsPerItem; ++j)
			{
				const size_t sourceIndex = bufferByteMetrics.absoluteBufferOffset + (i * bufferByteMetrics.numOfBytesPerItem) + (j * bufferByteMetrics.numOfBytesOfOneComponentOfItem);
				const size_t destIndex = (i * 4 * bufferByteMetrics.numOfComponentsPerItem) + (j*4);

				memcpy(&byteResult[destIndex], &buffer.data[sourceIndex], bufferByteMetrics.numOfBytesOfOneComponentOfItem);
			}
		}
	}
	extractedAttribute.rawData = byteResult;
	return extractedAttribute;

	
}

ExtractedVertexAttributeMap VertexAttributeRepacker::extractVertexAttributesFromInterleaved(VertexAttributeGltfLocationMap vertexAttributeGltfLocationMap, tinygltf::Model& tinygltfModel)
{

	std::vector<tinygltf::Accessor>& accessors = tinygltfModel.accessors;
	std::vector<tinygltf::BufferView>& bufferViews = tinygltfModel.bufferViews;
	std::vector<tinygltf::Buffer> buffers = tinygltfModel.buffers;

	ExtractedVertexAttributeMap extractedVertexAttributeMap;

	for (auto& vertexAttributeGltfLocation : vertexAttributeGltfLocationMap.vertexAttributeGltfLocationMap)
	{


		tinygltf::Accessor& accessor = accessors[vertexAttributeGltfLocation.second.accessorIndex];
		tinygltf::BufferView& bufferView = bufferViews[vertexAttributeGltfLocation.second.bufferViewIndex];

		BufferByteMetrics bufferByteMetrics = getBufferByteMetrics(accessor, bufferView);

		//using hardcoded index of buffer is bad idea , add correct indexing later
		extractedVertexAttributeMap.vertexAttributeMap[vertexAttributeGltfLocation.first] = extractAttributeFromInterleaved(buffers[0], bufferByteMetrics);

			//everything in byte count
			//extract (buffer, offset , stride , count)
	}



	return extractedVertexAttributeMap;
   
}

ExtractedVertexAttributeMap VertexAttributeRepacker::extractVertexAttributesFromNonInterleaved(VertexAttributeGltfLocationMap vertexAttributeGltfLocationMap, tinygltf::Model& tinygltfModel)
{
	std::vector<tinygltf::Accessor>& accessors = tinygltfModel.accessors;
	std::vector<tinygltf::BufferView>& bufferViews = tinygltfModel.bufferViews;
	std::vector<tinygltf::Buffer> buffers = tinygltfModel.buffers;

	ExtractedVertexAttributeMap extractedVertexAttributeMap;

	for (auto& vertexAttributeGltfLocation : vertexAttributeGltfLocationMap.vertexAttributeGltfLocationMap)
	{


		tinygltf::Accessor& accessor = accessors[vertexAttributeGltfLocation.second.accessorIndex];
		tinygltf::BufferView& bufferView = bufferViews[vertexAttributeGltfLocation.second.bufferViewIndex];

		BufferByteMetrics bufferByteMetrics = getBufferByteMetrics(accessor, bufferView);

		//using hardcoded index of buffer is bad idea , add correct indexing later
		extractedVertexAttributeMap.vertexAttributeMap[vertexAttributeGltfLocation.first] = extractAttributeFromNonInterleaved(buffers[0], bufferByteMetrics);

		//everything in byte count
		//ectract (buffer, offset , stride , count)
	}



	return extractedVertexAttributeMap;
}

InterleavedData VertexAttributeRepacker::interleaveAttributes(ExtractedVertexAttributeMap& extractedVertexAttributeMap, std::vector<Engine::VertexAttributes> packingFormat)
{


	
	size_t totalSizeOfBuffer = 0;
	size_t stride = 0;

	for (auto& attributeName : packingFormat)
	{
		ExtractedAttribute& extractedAttribute = extractedVertexAttributeMap.vertexAttributeMap.at(attributeName);
		const size_t numOfBytesPerItem = extractedAttribute.numOfBytesPerItem;
		totalSizeOfBuffer += numOfBytesPerItem * extractedAttribute.totalNumOfItems;

		stride += extractedAttribute.numOfBytesPerItem;
	}

	InterleavedData interleavedData;

	interleavedData.interleavedData.resize(totalSizeOfBuffer,std::byte(0b0000));
	interleavedData.stride = stride;

	std::vector<std::byte>& resultBuffer = interleavedData.interleavedData;

	for (auto& attributeName : packingFormat)
	{
		ExtractedAttribute& extractedAttribute = extractedVertexAttributeMap.vertexAttributeMap.at(attributeName);

		const size_t offset = m_attributeOffsetMap.at(attributeName);

		std::vector<std::byte>& attributeData = extractedAttribute.rawData;

		const size_t numOfBytePerComponent = extractedAttribute.numOfBytesPerItem;
		const size_t totalNumOfComponents = extractedAttribute.totalNumOfItems;

		for (size_t i = 0 ; i < totalNumOfComponents; ++i)
		{
			const size_t destIndex = offset + (i * stride);
			const size_t srcIndex = i * numOfBytePerComponent;
			//std::copy_n(&attributeData[i * numOfBytePerComponent], numOfBytePerComponent, &resultBuffer[offset + (i * (numOfBytePerComponent + stride))]);
			memcpy(&resultBuffer[destIndex], &attributeData[srcIndex], numOfBytePerComponent);
		}
	}





	return interleavedData;
}
