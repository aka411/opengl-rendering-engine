
#include "../include/geometry_generator.h"






GeometryGenerator::MeshData GeometryGenerator::getRectangle(const float length, const float breadth)
{

	//length along x axis
	//breadth along z axis
	const glm::vec3 unitSquare[6] =
	{
		{0,0,0},
		{1,1,0},
		{0,1,0},

		{0,0,0},
		{1,0,0},
		{1,1,0}
	};

	MeshData meshData;
	meshData.numOfVertex = 6;
	meshData.data.resize(sizeof(glm::vec3) * meshData.numOfVertex);

	//we need to scale the unit square to the desired length and breadth

	glm::vec3* vertexDataPtr = reinterpret_cast<glm::vec3*>(meshData.data.data());
	for (size_t i = 0; i < meshData.numOfVertex; ++i)
	{
		vertexDataPtr[i] = glm::vec3(unitSquare[i].x * length, unitSquare[i].y * breadth, unitSquare[i].z );
	}


	return meshData;

}

GeometryGenerator::MeshData GeometryGenerator::getRectangleWithOffset(const float length, const float breadth, const glm::vec3 offset)
{
	


	//length along x axis
	//breadth along z axis
	const glm::vec3 unitSquare[6] =
	{
		{0,0,0},
		{1,1,0},
		{0,1,0},

		{0,0,0},
		{1,0,0},
		{1,1,0}
	};

	MeshData meshData;
	meshData.numOfVertex = 6;
	meshData.data.resize(sizeof(glm::vec3) * meshData.numOfVertex);

	//we need to scale the unit square to the desired length and breadth


	std::vector<glm::vec3> resultVector(6);

	for (size_t i = 0; i < meshData.numOfVertex; ++i)
	{
		resultVector[i] = offset + glm::vec3(unitSquare[i].x * length, unitSquare[i].y * breadth, unitSquare[i].z);
	
	}

	memcpy(meshData.data.data(), resultVector.data(), sizeof(glm::vec3) * meshData.numOfVertex);

	return meshData;


}



GeometryGenerator::MeshData GeometryGenerator::getCuboidMesh(const float legnth, const float breadth, const float height)
{

	const glm::vec3 unitCube[36] =
	{
		//front face
		{0,0,0},
		{1,1,0},
		{0,1,0},
		{0,0,0},
		{1,0,0},
		{1,1,0},
		//back face
		{1,0,1},
		{0,1,1},
		{1,1,1},
		{1,0,1},
		{0,0,1},
		{0,1,1},
		//left face
		{0,0,1},
		{0,1,0},
		{0,1,1},
		{0,0,1},
		{0,0,0},
		{0,1,0},
		//right face
		{1,0,0},
		{1,1,1},
		{1,1,0},
		{1,0,0},
		{1,0,1},
		{1,1,1},
		//top face
		{0,1,0},
		{1,1,1},
		{0,1,1},
		{0,1,0},
		{1,1,0},
		{1,1,1},
		//bottom face
		{0,0,1},
		{1,0,0},
		{0,0,0},
		{0,0,1},
		{1,0,1},
		{1,0,0}

	};




	return MeshData();


}



GeometryGenerator::MeshData GeometryGenerator::getColouredRectangle(const float length, const float breadth, glm::vec4 colour)
{
	return MeshData();
}

GeometryGenerator::MeshData GeometryGenerator::getColouredCuboidMesh(const float legnth, const float breadth, const float height, glm::vec4 colour)
{
	return MeshData();
}

