#pragma once

#include <vector>
#include <glm/fwd.hpp>

class DataConvertor
{
private:

public:
	static glm::mat4 narrowToMatrix4(const std::vector<double>& doubleVector);

	static glm::vec3 narrowToVec3(const std::vector<double>& doubleVector);
	static glm::vec4 narrowToVec4(const std::vector<double>& doubleVector);
	

	static glm::quat narrowToQuat(const std::vector<double>& doubleVector);
};