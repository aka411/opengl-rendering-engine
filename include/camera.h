#pragma once
#include <glm/glm.hpp>
#include "glm/mat4x4.hpp"
#include <glm/ext/quaternion_float.hpp>
namespace Engine
{
	struct ViewPort
	{
		int width = 700;
		int height = 700;
	};

	class Camera
	{
	private:

		glm::mat4 m_projectionMatrix = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);




	public:

		ViewPort m_viewPort;

		glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
		glm::quat m_rotation{ 1.0f, 0.0f, 0.0f, 0.0f }; // w, x, y, z

		Camera() = default;

		const glm::mat4 getProjectionMatrix() const;


		void setProjectionMatrix(const glm::mat4& projectionMatrix);



		void setOrthographicProjection(float left, float right, float bottom, float top, float near, float far);

		void setPerspectiveProjection(float fov, float aspectRatio, float near, float far);

		//void move(float dx, float dy, float dz, float dT);

		void move(float dx, float dy, float dz);



		//getter for view matrix
		glm::mat4 getViewMatrix() const;

		void rotate(const float relX, const float relY);
		


	};

}