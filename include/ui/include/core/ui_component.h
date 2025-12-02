#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <vector>
#include "ui_core_data_structures.h"




namespace UI
{

	struct UIRenderMeshComponent
	{
		UIVertexFormat vertexFormat = UIVertexFormat::NORMAL_UI_VERTEX_FORMAT;
		UIBufferType bufferType = UIBufferType::STATIC;


		size_t vertexBufferOffset = 0;
		size_t vertexCount = 0;

		/*
		//DATA To Be Uploaded
		std::vector<std::byte> data;
		*/

		bool isDirty = true; // means data needs to be send to buffers


	};

	struct UIMaterialComponent
	{
		//To GPU buffer so ned to be careful about padding and alignment
		glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};
		glm::vec4 centerPos = {0.0f,0.0f,0.0f,1.0f}; // set by another system
		float radius = 0.5f;

		bool isDirty = true;
		
	};

	struct UIRectDimensionsComponent
	{
		float width = 100.0f;//in pixels
		float height = 100.0f; // in pixels

		bool isDirty = true;
	};

	/*
	struct UIRectBoundsComponent
	{
		glm::vec2 topLeft{0.0f};
		glm::vec2 bottomRight{0.0f};
	};
	*/

	struct UITransformComponent
	{
		glm::mat4 worldTransform = glm::identity<glm::mat4>();
		glm::mat4 localTransfrom = glm::identity<glm::mat4>();
		

		glm::vec3 position{0.0f};
		glm::vec3 scale{1.0f};
		glm::quat rotation{1.0f, 0.0f,0.0f,0.0f};

	};



	struct UITextComponent
	{
		float fontSize = 64;//in pixel units
		glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		std::string text;
		//use methods to set text etc so we can change flags correctly
		bool isDirty = true;
	};


	struct UITextMeshComponent
	{
		UIVertexFormat vertexFormat = UIVertexFormat::TEXT_UI_VERTEX_FORMAT;
		UIBufferType bufferType = UIBufferType::DYNAMIC;//actually more like streaming

		size_t vertexBufferOffset = 0;
		size_t vertexCount = 0;

	};

	struct UIRootComponent
	{
		//Marker component to identify root ui entities
	};




	struct UIHierarchyComponent
	{

	std::vector<UIEntity> children;

	};





	struct UIGraphComponent
	{//not the cleanest or logical code 
		//just experimenting to see what works

		glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		std::vector<float> buffer = std::vector<float>(40, 0.0f);

		size_t currentIndex = 0;
		float maxValue = 0.0f;
		size_t count = 0;
		void store(float dataPoint)
		{
			count++;
			if (count == 40)
			{
				count = 0;
				maxValue = dataPoint;
			}
			if (maxValue < dataPoint) maxValue = dataPoint;

			buffer[currentIndex++] = dataPoint;
			if (currentIndex == 40)
			{
				
				const size_t totalSize = buffer.size();
				std::vector<float> linearData(totalSize);
				memcpy(linearData.data(), &buffer[1], (totalSize-1) * sizeof(float));
			
				currentIndex--;
				buffer = std::move(linearData);
				
			}
		}

		std::vector<float> getData()
		{

			return buffer;
		}


	};




}