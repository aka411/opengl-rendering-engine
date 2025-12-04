#pragma once
#include <vector>


class Interleaver
{
private:


public:

	static std::vector<std::byte> interleave(const std::vector<std::vector<std::byte>>& srcDatas, const std::vector<size_t>& bytesPerSetOfData);
};