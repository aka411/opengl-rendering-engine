#pragma once
#include <vector>

class VertexAttributeRepacker
{
private:

public:

	static VertexData interleaveVertexAttributes(const std::map<VertexAttributeType, std::vector<std::byte>>& vertexAttributeToRawDatas);

};