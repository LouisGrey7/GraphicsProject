#include "Object.h"
#include <stb_image.h>

// Constructor
Object::Object(Camera* _cam, Mesh* _mesh, GLuint _program, LightManager* _LM, glm::vec3 _pos, bool _textured, std::string _textureFileName)
{
	//MESH CONSTRUCTOR
	this->m_sceneCamera = _cam;
	this->m_mesh = _mesh;
	this->m_program = _program;
	this->m_textureID = CreateTexture(_textureFileName);
	this->m_lightManager = _LM;

	m_textured = _textured;
	Position = _pos;
	m_rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
}

Object::Object(Camera* _cam, Mesh* _mesh, GLuint _program, LightManager* _LM, glm::vec3 _pos, bool _textured, GLuint _textureFileName)
{
	//MESH CONSTRUCTOR
	this->m_sceneCamera = _cam;
	this->m_mesh = _mesh;
	this->m_program = _program;
	this->m_textureID = _textureFileName;
	this->m_lightManager = _LM;

	m_textured = _textured;
	Position = _pos;
	m_rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Object::Object(Camera* _cam, LightManager* _LM, GLuint _program, glm::vec3 _pos, std::string _folderPath, std::string _meshFileName, std::string _textureFileName, float _fogAmount)
{

	//MODEL CONSTRUCTOR

	this->m_sceneCamera = _cam;
	this->m_mesh = new MeshModel(_folderPath, _meshFileName);
	this->m_program = _program;
	this->m_lightManager = _LM;
	this->m_textureID = CreateTexture(_folderPath, _textureFileName);

	//Transforms
	Position = _pos;
	m_rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(0.1f, 0.1f, 0.1f);


	m_fogAmount = _fogAmount;

	m_textured = true;
	
}

Object::Object(Camera* _cam, LightManager* _LM, GLuint _program, glm::vec3 _pos, std::string _folderPath, std::string _meshFileName, std::string _textureFileName, float _fogAmount, GLuint _textureID)
{

	//MODEL CONSTRUCTOR with 3D Texture

	this->m_sceneCamera = _cam;
	this->m_mesh = new MeshModel(_folderPath, _meshFileName);
	this->m_program = _program;
	this->m_lightManager = _LM;
	this->m_textureID = _textureID;

	//Transforms
	Position = _pos;
	m_rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(0.1f, 0.1f, 0.1f);


	m_fogAmount = _fogAmount;

	m_textured = true;

}



Object::~Object()
{
	delete m_mesh;
}

void Object::Update(float _deltaTime)
{
	// Calculate Model Matrix
	m_matTranslation = glm::translate(glm::mat4(), Position);
	m_matRotation = glm::rotate(glm::mat4(), glm::radians(m_rotationAngle.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_matScale = glm::scale(glm::mat4(), m_scale);
	m_matModel = m_matTranslation * m_matRotation * m_matScale;
	m_matPVM = m_sceneCamera->GetMatrixPV() * m_matModel;
}

void Object::Render()
{
	if (m_sceneCamera == nullptr)
	{
		return;
	}

	glUseProgram(m_program);

	//Pass uniforms for Vertex Shader
	GLint PVMLoc = glGetUniformLocation(m_program, "PVM");
	glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(m_matPVM));
	GLint modelLoc = glGetUniformLocation(m_program, "Model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(m_matModel));
	
	switch (m_programMode)
	{
		case ProgramMode::BLINNPHONGRIM:
		{
			//Pass uniforms for Frag Shader
			GLint m_cameraPos = glGetUniformLocation(m_program, "CameraPos");
			glUniform3fv(m_cameraPos, 1, glm::value_ptr(m_sceneCamera->GetCameraPosition()));

	
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_textureID);
			glUniform1i(glGetUniformLocation(m_program, "ImageTexture0"), 0);

			//Light uniforms for FragShader
			glUniform3fv(glGetUniformLocation(m_program, ("PointLights[0].Position")), 1, glm::value_ptr(m_lightManager->m_PointLights[0].position));
			glUniform3fv(glGetUniformLocation(m_program, "PointLights[0].Color"), 1, glm::value_ptr(m_lightManager->m_PointLights[0].color));
			glUniform1f(glGetUniformLocation(m_program, "PointLights[0].AmbientStrength"), m_lightManager->m_PointLights[0].ambientStrength);
			glUniform1f(glGetUniformLocation(m_program, "PointLights[0].SpecularStrength"), m_lightManager->m_PointLights[0].specularStrength);
			glUniform1f(glGetUniformLocation(m_program, "PointLights[0].AttenuationConstant"), m_lightManager->m_PointLights[0].attenuationConstant);
			glUniform1f(glGetUniformLocation(m_program, "PointLights[0].AttenuationLinear"), m_lightManager->m_PointLights[0].attenuationLinear);
			glUniform1f(glGetUniformLocation(m_program, "PointLights[0].AttenuationExponent"), m_lightManager->m_PointLights[0].attenuationExponent);


			GLint lightShininess = glGetUniformLocation(m_program, "Shininess");
			glUniform1f(lightShininess, m_shininess);

			GLint fogAmount = glGetUniformLocation(m_program, "FogAmount");
			glUniform1f(fogAmount, m_fogAmount);
			break;
		}
		case ProgramMode::UNTEXTURED:
		{
			glUniform3fv(glGetUniformLocation(m_program, "ColorIn"), 1, glm::value_ptr(m_color));
			break;
		}
		case OUTLINED: 
		{
			glUniformMatrix4fv(glGetUniformLocation(m_program, "camMatrix"), 1, GL_FALSE, glm::value_ptr(m_sceneCamera->GetMatrixPV()));
			glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, GL_FALSE, glm::value_ptr(m_matModel));
			glUniformMatrix4fv(glGetUniformLocation(m_program, "translation"), 1, GL_FALSE, glm::value_ptr(m_matTranslation));
			glUniformMatrix4fv(glGetUniformLocation(m_program, "rotation"), 1, GL_FALSE, glm::value_ptr(m_matRotation));
			glUniformMatrix4fv(glGetUniformLocation(m_program, "scale"), 1, GL_FALSE, glm::value_ptr(m_matScale));
			glUniform1f(glGetUniformLocation(m_program, "outlining"), 2.0f);


			break;
		}
		default:
		{
			break;
		}
	}
	

	m_mesh->Render();

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Object::SetColor(glm::vec3 _color)
{
	m_color = _color;
}

void Object::SetScale(glm::vec3 _scale)
{
	m_scale = _scale;
}

void Object::SetRotation(float _rot)
{
	m_rotationAngle.y = _rot;
}

void Object::SetPosition(glm::vec3	_pos)
{

	Position = _pos;


}

void Object::SetMode(ProgramMode _programMode)
{
	m_programMode = _programMode;
}

void Object::SetProgram(GLuint _program)
{
	m_program = _program;
}

GLuint Object::CreateTexture(std::string _fileName)
{
	GLuint textureID;
	std::string fullFilePath = "Resources/Textures/";
	fullFilePath.append(_fileName);

	// Load the Image data
	int imageWidth;
	int imageHeight;
	int imageComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(fullFilePath.c_str(),
		&imageWidth, &imageHeight, &imageComponents, 0);

	// Create and bind a new texture template
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Check how many components the loaded image has (RGB or RGBA?)
	GLint loadedComponents = (imageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image data
	glTexImage2D(GL_TEXTURE_2D, 0, loadedComponents, imageWidth, imageHeight, 0,
		loadedComponents, GL_UNSIGNED_BYTE, imageData);

	// Setting the address mode for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate the mipmaps, free the memory and unbind the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint Object::CreateTexture(std::string _filePath, std::string _fileName)
{
	GLuint textureID;
	std::string fullFilePath = _filePath;
	fullFilePath.append(_fileName);

	// Load the Image data
	int imageWidth;
	int imageHeight;
	int imageComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(fullFilePath.c_str(),
		&imageWidth, &imageHeight, &imageComponents, 0);

	// Create and bind a new texture template
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Check how many components the loaded image has (RGB or RGBA?)
	GLint loadedComponents = (imageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image data
	glTexImage2D(GL_TEXTURE_2D, 0, loadedComponents, imageWidth, imageHeight, 0,
		loadedComponents, GL_UNSIGNED_BYTE, imageData);

	// Setting the address mode for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate the mipmaps, free the memory and unbind the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

