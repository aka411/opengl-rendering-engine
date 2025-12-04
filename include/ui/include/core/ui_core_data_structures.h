#pragma once

#include "../../../../external/the-engine/the-engine/ecs/include/common_data_types.h"

namespace UI
{




//TODO : Might need to add better format types later

//a shader is defined by base what it can do,
// what attributes it can take, 
// what uniforms it can take, 
// what specific binding slots it can take
//what all extra data it needs in form of either ssbo or uniorm buffers
//standardizing this is hard , 
//for now we will hardcode it



	enum class UIVertexFormat
	{
		NORMAL_UI_VERTEX_FORMAT = 0, // Position , Colour
		TEXT_UI_VERTEX_FORMAT = 1 // Position , UV ,Colour

	};

	enum class UIBufferType
	{
		STATIC,
		STREAMING,
		DYNAMIC

	};




	//having material and rect bounds make it a renderable ui entity
	using UIEntity = TheEngine::ECS::EntityId;






}