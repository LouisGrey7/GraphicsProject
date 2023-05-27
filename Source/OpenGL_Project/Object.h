#pragma once

#include <glew.h>
#include <glfw3.h>

#include "LightManager.h"
#include "Camera.h"
#include "Sphere.h"
#include "MeshModel.h"
#include "Utility.h"
#include "Quad.h"
#include "PointMesh.h"
#include "QuadPatch.h"

enum ProgramMode
{
	UNTEXTURED,
	BLINNPHONGRIM,
	OUTLINED
};

class Object
{
private:
	Mesh* m_mesh;
	Camera* m_sceneCamera;
	LightManager* m_lightManager;
	GLuint m_program;
	GLuint m_textureID;

	float m_shininess = 50.0f;
	
	float m_fogAmount = 0.0f;

	glm::vec3 Position;
	glm::vec3 m_scale;
	glm::vec3 m_rotationAngle;

	glm::mat4 m_matTranslation;
	glm::mat4 m_matRotation;
	glm::mat4 m_matScale;
	glm::mat4 m_matModel;
	glm::mat4 m_matPVM;

	glm::vec3 m_color;
	bool m_textured;

	float m_rimExponent = 3.0f;
	bool m_rimmed = false;

public:
	Object(Camera* _cam, Mesh* _mesh, GLuint _program, LightManager* _LM, glm::vec3 _pos, bool _textured, std::string _textureFileName);
	Object(Camera* _cam, Mesh* _mesh, GLuint _program, LightManager* _LM, glm::vec3 _pos, bool _textured, GLuint _textureFileName);
	Object(Camera* _cam, LightManager* _LM, GLuint _program, glm::vec3 _pos, std::string _folderPath, std::string _meshFileName, std::string _textureFileName, float _fogAmount);
	Object(Camera* _cam, LightManager* _LM, GLuint _program, glm::vec3 _pos, std::string _folderPath, std::string _meshFileName, std::string _textureFileName, float _fogAmount, GLuint _textureID);
	~Object();

	enum ProgramMode m_programMode = ProgramMode::BLINNPHONGRIM;

	void Update(float _deltaTime);
	void Render();

	void SetColor(glm::vec3 _color);
	void SetScale(glm::vec3 _scale);
	void SetRotation(float _rot);
	void SetPosition(glm::vec3 _pos);


	void SetMode(ProgramMode _programMode);
	void SetProgram(GLuint _program);

	void SetRimmed() { m_rimmed = true; };
	bool GetRimmed() { return m_rimmed; };

	GLuint CreateTexture(std::string _fileName);
	GLuint CreateTexture(std::string _filePath, std::string _fileName);


};

