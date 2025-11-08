#include "../include/data_convertor.h"

glm::mat4 DataConvertor::narrowToMatrix4(const std::vector<double>& doubleVector)
{

	glm::mat4 result{};
	assert(doubleVector.size() == 16);

	for (int i = 0; i < doubleVector.size(); ++i)
	{
		// Cast the double to float for storage in glm::mat4
		result[i / 4][i % 4] = static_cast<float>(doubleVector[i]);


	}
	return result;

}



glm::vec4 DataConvertor::narrowToVec4(const std::vector<double>& doubleVector)
{

	glm::vec4 result{};
	assert(doubleVector.size() == 4);
	for (int i = 0; i < doubleVector.size(); ++i)
	{
		// Cast the double to float for storage in glm::mat4
		result[i] = static_cast<float>(doubleVector[i]);
	}
	return result;
}

glm::vec3 DataConvertor::narrowToVec3(const std::vector<double>& doubleVector)
{

	glm::vec3 result{};
	assert(doubleVector.size() == 3);
	for (int i = 0; i < doubleVector.size(); ++i)
	{
		// Cast the double to float for storage in glm::mat4
		result[i] = static_cast<float>(doubleVector[i]);
	}
	return result;
}

glm::quat DataConvertor::narrowToQuat(const std::vector<double>& doubleVector)
{
	glm::quat result{};
	assert(doubleVector.size() == 4);
	for (int i = 0; i < doubleVector.size(); ++i)
	{
		// Cast the double to float for storage in glm::mat4
		result[i] = static_cast<float>(doubleVector[i]);
	}
	return result;
}