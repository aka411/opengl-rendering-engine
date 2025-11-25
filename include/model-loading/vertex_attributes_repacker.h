#pragma once
#include <vector>
#include <map>

#include <cstddef>
#include "model_loading_data_structure.h"


class VertexAttributeRepacker
{
private:

public:

	static VertexData interleaveVertexAttributes(const ExtractedAttributeData& extractedAttributeData);

};