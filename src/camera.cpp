#include "../include/camera.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Engine
{


	void Camera::setProjectionMatrix(const glm::mat4& projectionMatrix)
	{
		m_projectionMatrix = projectionMatrix;
	}

	const glm::mat4 Camera::getProjectionMatrix() const
	{
		return m_projectionMatrix;
	}


	void Camera::setOrthographicProjection(float left, float right, float bottom, float top, float near, float far)
	{





		m_projectionMatrix = glm::mat4(
			2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
			0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
			0.0f, 0.0f, 2.0f / (near - far), -(near + far) / (near - far),
			0.0f, 0.0f, 0.0f, 1.0f);


	}


	void Camera::setPerspectiveProjection(float fov, float aspectRatio, float near, float far)
	{




		m_projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, near, far);





	}


	void Camera::move(float dx, float dy, float dz)
	{

		const glm::vec3 localRight = m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3 localUp = m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3 localForward = m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);

		
		m_position += localRight * dx  ;
		m_position += localUp * dy ;
		m_position += localForward * dz ;

	}





	glm::mat4 Camera::getViewMatrix() const
	{
		




		return glm::inverse(  glm::translate(glm::mat4(1.0f), m_position) * glm::mat4_cast(m_rotation));
		



	}

	void Camera::rotate(const float relX, const float relY)
	{



		const glm::vec3 worldY(0.0f, 1.0f, 0.0f);

		const float sensitivity = 90.0f;
		const float yawAngle = (relX / m_viewPort.width) * sensitivity;
		const float pitchAngle = (relY / m_viewPort.height) * sensitivity;



		glm::quat camX = m_rotation * glm::quat(0.0f, 1.0f, 0.0f, 0.0f) * glm::inverse(m_rotation);
		const glm::vec3 localRight = glm::vec3(camX.x, camX.y, camX.z);


		glm::quat qYaw = glm::angleAxis(glm::radians(-yawAngle), worldY);

		
		glm::quat qPitch = glm::angleAxis(glm::radians(-pitchAngle), localRight);


		glm::quat qInc = qYaw * qPitch;

		m_rotation = qInc * m_rotation;

		m_rotation = glm::normalize(m_rotation);

	}

}