#include "Camera.h"

#include "Utility.h"

Camera::Camera(Projection Type)
{
	m_projectionType = Type;
	switch (Type)
	{
		case ORTHOGRAPHIC:
		{
			
		}
		break;
		case PERSPECTIVE:
		{
			m_matProjection = glm::perspective(glm::radians(45.0f), (float)Utils::WindowWidth / (float)Utils::WindowHeight, 0.1f, 4000.0f);
		}
		break;
		default:
		{

		}
	}

	m_timeElapsed = 0.0f;
}

Camera::~Camera()
{

}

void Camera::Update(float DeltaTime)
{
	if (m_orbitalCamera)
	{
		m_timeElapsed += DeltaTime;
		m_Position.x = sin(m_timeElapsed) * m_cameraOrbitRadius;
		m_Position.y = n_cameraOrbitHeight;
		m_Position.z = cos(m_timeElapsed) * m_cameraOrbitRadius;
	}

	m_matView = glm::lookAt(m_Position, (m_Position + m_CameraFront), m_UpDirection);
	m_matPV = m_matProjection * m_matView;
}

void Camera::CameraLook()
{
	
	glm::vec3 front;
	front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	front.y = sin(glm::radians(m_Pitch));
	front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_CameraFront = glm::normalize(front);
}
