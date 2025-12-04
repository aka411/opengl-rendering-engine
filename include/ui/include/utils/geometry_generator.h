#pragma once
#include <glm/glm.hpp>
#include <vector>






class GeometryGenerator
{

private:


public:
	//need more thought here
	struct MeshData
	{
		//I think i need to add format info here too

		size_t numOfVertex = 0;
		std::vector<std::byte> data;
	};

	static MeshData getRectangle(const float length, const float breadth); // ablity to add offset later
	static MeshData getRectangleWithOffset(const float length, const float breadth,const glm::vec3 offset);

	static MeshData getCuboidMesh(const float legnth,const float breadth, const float height);



	//MeshData?
	static MeshData getColouredRectangle(const float length, const float breadth, glm::vec4 colour);
	static MeshData getColouredRectangleWithOffset(const float length, const float breadth, const glm::vec3 offset, const glm::vec4 colour);

	static MeshData getColouredCuboidMesh(const float legnth, const float breadth, const float height,glm::vec4 colour);


};