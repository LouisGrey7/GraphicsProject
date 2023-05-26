#pragma once

#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

enum Projection
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

class Camera
{
private:
	
	Projection m_projectionType;
	glm::mat4 m_matView;
	glm::mat4 m_matProjection;
	glm::mat4 m_matPV;

	bool m_orbitalCamera = false;
	float m_timeElapsed;
	float m_cameraOrbitRadius = 3.0f;
	float n_cameraOrbitHeight = 2.0f;


public:
	Camera(Projection Type);
	~Camera();

	void Update(float DeltaTime);
	glm::mat4& GetMatrixPV() { return m_matPV; };
	glm::vec3 GetCameraPosition() { return m_Position; };
	void CameraLook();


	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 3.0f);;
	glm::vec3 m_Target = glm::vec3(0.0f, 0.0f, 0.0f);;
	glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);;
	glm::vec3 m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);;

	float m_Yaw = -90.0;
	float m_Pitch = 0.0;

};

