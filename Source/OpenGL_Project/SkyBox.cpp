#include "SkyBox.h"
#include <stb_image.h>

SkyBox::SkyBox(Camera* _cam, GLuint _program, std::string _fileName, float _fogAmount)
{	
	m_fogAmount = _fogAmount;

	this->m_camera = _cam;
	this->m_program = _program;

	float vertices[] =
	{
		//   Coordinates
		-1.0f, -1.0f,  1.0f,//        7--------6
		 1.0f, -1.0f,  1.0f,//       /|       /|
		 1.0f, -1.0f, -1.0f,//      4--------5 |
		-1.0f, -1.0f, -1.0f,//      | |      | |
		-1.0f,  1.0f,  1.0f,//      | 3------|-2
		 1.0f,  1.0f,  1.0f,//      |/       |/
		 1.0f,  1.0f, -1.0f,//      0--------1
		-1.0f,  1.0f, -1.0f
	};

	unsigned int indices[] =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4,
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5,
		5, 4, 0,
		// Front
		3, 7, 6,
		6, 2, 3
	};
	
	//Generate, bind and fill buffers
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	

	//Image data init
	int imageWidth;
	int imageHeight;
	int imageComponents;

	std::string textureSuffix[6] =
	{
		"Right.jpg",
		"Left.jpg",
		"Up.jpg",
		"Down.jpg",
		"Back.jpg",
		"Front.jpg"
	};

	//Generate the mipmaps free the memory and unbind the texture
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{	
		std::string fullPath = "Resources/Textures/Cubemaps/" + _fileName + "/" + textureSuffix[i];
		unsigned char* imageData = stbi_load(fullPath.c_str(), &imageWidth, &imageHeight, &imageComponents, 0);

		//Test Component num
		GLint loadedComponents = (imageComponents == 4) ? GL_RGBA : GL_RGB;

		stbi_set_flip_vertically_on_load(false);
		//Populate the texture with the loaded image;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, loadedComponents, imageWidth, imageHeight, 0, 
			loadedComponents, GL_UNSIGNED_BYTE, imageData);
		
		stbi_image_free(imageData);
	}

}

SkyBox::~SkyBox()
{
}

void SkyBox::Update(float _deltaTime)
{

	m_matModel = glm::scale(glm::mat4(), glm::vec3(2000.0f, 2000.0f, 2000.0f));
	m_matPVM = m_camera->GetMatrixPV() * m_matModel;

}

void SkyBox::Render()
{
	glDepthFunc(GL_LEQUAL);

	glUseProgram(m_program);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	GLint modelLoc = glGetUniformLocation(m_program, "Model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_matModel));

	GLint m_cameraPos = glGetUniformLocation(m_program, "CameraPos");
	glUniform3fv(m_cameraPos, 1, glm::value_ptr(m_camera->GetCameraPosition()));

	glUniform1i(glGetUniformLocation(m_program, "Texture0"), 0);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "PV"), 1, GL_FALSE, glm::value_ptr(m_matPVM));

	GLint fogAmount = glGetUniformLocation(m_program, "FogAmount");
	glUniform1f(fogAmount, m_fogAmount);

	glBindVertexArray(m_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	glDepthFunc(GL_LESS);

}

