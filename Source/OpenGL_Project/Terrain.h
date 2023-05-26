#pragma once
#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "VertexFormat.h"
#include "Camera.h"
#include "LightManager.h"


struct HeightMapInfo
{
	std::string FilePath;
	unsigned int Width;
	unsigned int Depth;
	float HeightScale;
	float HeightOffset;
	float CellSpacing;


};

class Terrain
{
private:
	HeightMapInfo m_info;
	GLuint m_VAO;
	int m_indexCount;

	GLuint m_texture;

	GLuint m_program;
	LightManager* m_lightManager;
	Camera* m_camera;
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


	std::vector<float> m_heightMap;

public:

	Terrain(Camera* _camera, GLuint _program, LightManager* _lightManager, glm::vec3 _pos,
		std::string _folderPath, std::string _textureFileName, float _fogAmount, std::string _mapFilePath);
	~Terrain();


	void Update(float _deltaTime);
	void Render();
	bool LoadHeightMap();
	GLuint CreateTexture(std::string _filePath, std::string _fileName);

	float Average(unsigned int row, unsigned int col);

};

