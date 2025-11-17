#pragma once
#include <vector>
#include "../low-level/rendering_system_data_types.h"
#include "../low-level/gpu_material_system.h"
#include "../engine_data_structures(outdated).h"








struct VertexData
{

	Vertexlayout vertexLayout;
	VertexFormat vertexFormat;

	//Interleaved Only
	std::vector<std::byte> data;
};


struct IntermediateMesh
{
	std::vector<IntermediatePrimitive> intermediatePrimitive;
};
struct IntermediatePrimitive
{
	int vertexDataId = -1; //needs to be converted to offset
	size_t count = 0;
	bool isIndexed = false;
	int indexId = -1; //need to be converted into offset

	int materialId = -1;//
};

//index data is gonna be a bitch

struct EngineIntermediateNode
{
	Engine::Transformation transformation;
	int meshIndex = -1;//index to mesh list
	std::vector<EngineIntermediateNode> children;
};

struct EngineIntermediateModel
{
	std::vector<VertexData> vertexDatas;
	std::vector<PBRMetallicRoughnessMaterial> intermdediatePBRMetallicRoughnessMaterials; // PBR // use unordered map
	std::vector<TextureCreateInfo> TextureCreateInfos;//use unordered map

	std::vector<IntermediateMesh> intermediateMeshs;



	/**CPU DATA**/




};