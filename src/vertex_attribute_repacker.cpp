#include "../include/vertex_attribute_repacker.h"






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
	 {Engine::VertexAttributes::TEXCOORD_0, 24}


 };


BufferByteMetrics VertexAttributeRepacker::getBufferByteMetrics(tinygltf::Accessor& accessor, tinygltf::BufferView& bufferView)
{
	BufferByteMetrics bufferByteMetrics;

	bufferByteMetrics.absoluteBufferOffset = accessor.byteOffset + bufferView.byteOffset;
	bufferByteMetrics.numOfBytePerComponent = m_typeToNumOfComponentsMap.at(accessor.type) * m_componentTypeToByteSizeMap.at(accessor.componentType);
	
	bufferByteMetrics.totalBytes = accessor.count * bufferByteMetrics.numOfBytePerComponent;
	//bufferByteMetrics.byteStride = (bufferView.byteStride > 0 )? bufferView.byteStride : 0 ;

	bufferByteMetrics.totalNumOfComponents = accessor.count;
	return bufferByteMetrics;
}

ExtractedAttribute VertexAttributeRepacker::extractAttributeFromInterleaved(tinygltf::Buffer& buffer, const BufferByteMetrics& bufferByteMetrics)
{
	const size_t sizeOfBufferInbytes = buffer.data.size();
	//const unsigned char* byteBufferPtr = reinterpret_cast<const unsigned char*>(buffer.data);
 
	 ExtractedAttribute extractedAttribute;
	 extractedAttribute.totalNumOfComponents = bufferByteMetrics.totalNumOfComponents;
	 extractedAttribute.numOfBytePerComponent = bufferByteMetrics.numOfBytePerComponent;


	 std::vector<std::byte> byteResult(bufferByteMetrics.totalBytes);

	
	 for (size_t i = 0; i < bufferByteMetrics.totalNumOfComponents; ++i)
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
	extractedAttribute.totalNumOfComponents = bufferByteMetrics.totalNumOfComponents;
	extractedAttribute.numOfBytePerComponent = bufferByteMetrics.numOfBytePerComponent;


	std::vector<std::byte> byteResult(bufferByteMetrics.totalBytes);




		const size_t sourceIndex = bufferByteMetrics.absoluteBufferOffset;
		
		memcpy(&byteResult[0], &buffer.data[sourceIndex], bufferByteMetrics.totalBytes);//will it also copy padding?
	
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

std::vector<std::byte> VertexAttributeRepacker::interleaveAttributes(ExtractedVertexAttributeMap& extractedVertexAttributeMap, std::vector<Engine::VertexAttributes> packingFormat)
{


	
	size_t totalSizeOfBuffer = 0;
	size_t stride = 0;

	for (auto& attributeName : packingFormat)
	{
		ExtractedAttribute& extractedAttribute = extractedVertexAttributeMap.vertexAttributeMap.at(attributeName);
		totalSizeOfBuffer += extractedAttribute.numOfBytePerComponent * extractedAttribute.totalNumOfComponents;

		stride += extractedAttribute.numOfBytePerComponent;
	}

	std::vector<std::byte> resultBuffer(totalSizeOfBuffer);




	for (auto& attributeName : packingFormat)
	{
		ExtractedAttribute& extractedAttribute = extractedVertexAttributeMap.vertexAttributeMap.at(attributeName);

		const size_t offset = m_attributeOffsetMap.at(attributeName);

		std::vector<std::byte>& attributeData = extractedAttribute.rawData;

		const size_t numOfBytePerComponent = extractedAttribute.numOfBytePerComponent;
		const size_t totalNumOfComponents = extractedAttribute.totalNumOfComponents;

		for (size_t i = 0 ; i < totalNumOfComponents; ++i)
		{
			const size_t destIndex = offset + (i * stride);
			const size_t srcIndex = i * numOfBytePerComponent;
			//std::copy_n(&attributeData[i * numOfBytePerComponent], numOfBytePerComponent, &resultBuffer[offset + (i * (numOfBytePerComponent + stride))]);
			memcpy(&resultBuffer[destIndex], &attributeData[srcIndex], numOfBytePerComponent);
		}
	}





	return resultBuffer;
}
