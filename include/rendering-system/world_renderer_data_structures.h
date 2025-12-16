#pragma once
#include <cstdint>
#include "../components.h"

namespace Engine
{



	/******NOT COMPONENTS****/
	struct ObjectData
	{
	
		glm::mat4 modelMatrix;// 64 bytes
		uint32_t materialId;// my gpu driver does not support uint64 

		uint32_t boneId; // 4 bytes
		
		float padding[2];
	};


	/*
	struct RenderCommand
	{

		size_t	vertexOffset = 0;//in bytes
		size_t	vertexCount = 0;

		
		bool isIndexed = false;
		IndexType indexType;
		size_t	indexOffset = 0;
		size_t	indexCount = 0;



		ObjectData objectData;

	};
	*/
	/*
	// Struct for MultiDrawArrays
	typedef  struct
	{
		unsigned int  count;
		unsigned int  instanceCount;
		unsigned int  firstVertex;
		unsigned int  baseInstance;
		// Optional user-defined data goes here - if nothing, stride is 0
	} DrawArraysIndirectCommand;
	// sizeof(DrawArraysIndirectCommand) == 16

	// Struct for MultiDrawElements
	typedef  struct
	{
		unsigned int  count;
		unsigned int  instanceCount;
		unsigned int  firstIndex;
		int           baseVertex;
		unsigned int  baseInstance;
		// Optional user-defined data goes here - if nothing, stride is 0
	} DrawElementsIndirectCommand;
	// sizeof(DrawElementsIndirectCommand) == 20
	*/

	struct IndirectArrayRenderCommand
	{

		unsigned int  count;
		unsigned int  instanceCount;
		unsigned int  firstVertex;
		unsigned int  baseInstance;



	};


	struct IndirectIndexedRenderCommand
	{
		unsigned int  count;
		unsigned int  instanceCount;
		unsigned int  firstIndex;
		int           baseVertex;
		unsigned int  baseInstance;

	};


}