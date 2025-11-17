#pragma once



class WorldVertexBufferSystem
{

private:
	std::unordered_map<VertexFormat,GPUBufferSubBumpAllocator> m_formatToBufferSubAllocators;

public:

	WorldVertexBufferSystem();
	//get buffer for given vertex format

};