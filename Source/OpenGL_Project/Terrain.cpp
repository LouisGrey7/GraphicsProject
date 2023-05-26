#include "Terrain.h"
#include <stb_image.h>

Terrain::Terrain(Camera* _camera, GLuint _program, LightManager* _lightManager, glm::vec3 _pos, 
	std::string _folderPath, std::string _textureFileName, float _fogAmount , std::string _mapFilePath)
{
	m_color = glm::vec3(0.4f, 0.4f, 0.4f);

	this->m_camera = _camera;
	this->m_program = _program;
	this->m_lightManager = _lightManager;

	this->m_textureID = CreateTexture(_folderPath, _textureFileName);

	m_fogAmount = _fogAmount;



	Position = _pos;
	m_rotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

	m_info.FilePath = _mapFilePath;
	m_info.Width = 512.0f;
	m_info.Depth = 512.0f;
	m_info.HeightScale = 1.0f;
	m_info.HeightOffset = 1.0f;
	m_info.CellSpacing = 1.0f;

	LoadHeightMap();


	//Build Vertices
	unsigned int vertexCount = m_info.Width * m_info.Depth; 
	std::vector<VertexFormat> vertices(vertexCount);

	float halfWidth = (m_info.Width - 1) * m_info.CellSpacing * 0.5f; 
	float halfDepth = (m_info.Depth - 1) * m_info.CellSpacing * 0.5f;

	float texU = 4.0f / (float)(m_info.Width - 1);
	float texV = 4.0f / (float)(m_info.Depth - 1);

	for (unsigned int row = 0; row < m_info.Width; row++)
	{
		float posZ = halfDepth - (row * m_info.CellSpacing);

		for (unsigned int col = 0; col < m_info.Depth; col++)
		{
			int index = row * m_info.Width + col;

			float posX = -halfWidth + (col * m_info.CellSpacing); 
			float posY = m_heightMap[index];

			vertices[index].Position = glm::vec3(posX, posY, posZ);
			vertices[index].TexCoord = glm::vec2(col * texU, row * texV);
			vertices[index].Normal = glm::vec3(0.0f, 1.0f, 0.0f);

		}
	}


	// Calculate normals using central difference
	float invCellSpacing = 1.0f / (2.0f * m_info.CellSpacing); 
	for (unsigned int row = 2; row < m_info.Width - 1; row++)
	{
		for (unsigned int col = 2; col < m_info.Depth - 1; col++)
		{

			float rowNeg = m_heightMap[(row - 1) * m_info.Depth + col];
			float rowPos = m_heightMap[(row + 1) * m_info.Depth + col];
			float colNeg = m_heightMap[row * m_info.Depth + col - 1];
			float colPos = m_heightMap[row * m_info.Depth + col + 1];

			glm::vec3 tanZ(0.0f, (rowNeg - rowPos) * invCellSpacing, 1.0f);
			glm::vec3 tanX(1.0f, (colPos - colNeg) * invCellSpacing, 0.0f);

			glm::vec3 normal = glm::cross(tanZ, tanX);
			normal = glm::normalize(normal);

			vertices[row * m_info.Depth + col].Normal = normal;

		}
	}


	unsigned int faceCount = (m_info.Width - 1) * (m_info.Depth - 1) * 2;
	m_indexCount = faceCount * 3;
	std::vector<GLuint> indices(m_indexCount);

	//Build Indices
	int index = 0;
	for (unsigned int row = 0; row < (m_info.Width - 1); row++)
	{

		for (unsigned int col = 0; col < (m_info.Depth - 1); col++)
		{
			indices[index++] = row * m_info.Depth + col;
			indices[index++] = row * m_info.Depth + col + 1;
			indices[index++] = (row + 1) * m_info.Depth + col;

			indices[index++] = (row + 1) * m_info.Depth + col;
			indices[index++] = row * m_info.Depth + col + 1;
			indices[index++] = (row + 1) * m_info.Depth + col + 1;


		}
	}


	GLuint VBO, EBO;
	//Build VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Build VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::Position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::TexCoord)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::Normal)));
	glEnableVertexAttribArray(2);

	std::cout << "Vertex Size: " << m_indexCount << std::endl;


}

Terrain::~Terrain()
{
}

void Terrain::Update(float _deltaTime)
{
	m_matTranslation = glm::translate(glm::mat4(), Position);
	m_matRotation = glm::rotate(glm::mat4(), glm::radians(m_rotationAngle.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_matScale = glm::scale(glm::mat4(), m_scale);
	m_matModel = m_matTranslation * m_matRotation * m_matScale;

	m_matPVM = m_camera->GetMatrixPV() * m_matModel;
}

void Terrain::Render()
{

	if (m_camera == nullptr)
	{
		return;
	}

	glUseProgram(m_program);

	//Pass uniforms for Vertex Shader
	GLint PVMLoc = glGetUniformLocation(m_program, "PVM");
	glUniformMatrix4fv(PVMLoc, 1, GL_FALSE, glm::value_ptr(m_matPVM));
	GLint ModelLoc = glGetUniformLocation(m_program, "Model");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(m_matModel));


	//Pass uniforms for Frag Shader
	GLint m_cameraPos = glGetUniformLocation(m_program, "CameraPos");
	glUniform3fv(m_cameraPos, 1, glm::value_ptr(m_camera->GetCameraPosition()));


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

	glBindVertexArray(m_VAO);

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

}

bool Terrain::LoadHeightMap()
{
	unsigned int vertexCount = m_info.Width * m_info.Depth;

	//Struct contains height info
	std::vector <unsigned char> heightValue(vertexCount);


	//Open data file and read
	std::ifstream File;
	File.open(m_info.FilePath, std::ios_base::binary);
	if (File)
	{

		//Read in raw bytes
		File.read((char*)&heightValue[0], (std::streamsize)heightValue.size());
		File.close(); //File no longer needed
		std::cout << "Height Map Loaded" << std::endl;

	}
	else
	{

		std::cout << "Error: Height map failed to load: " << m_info.FilePath << std::endl;
		return false;
	}

	
	//Transfer data into float array. Apply Scaling and offset
	m_heightMap.resize(vertexCount, 0);
	for (unsigned int i = 0; i < vertexCount; i++)
	{

		m_heightMap[i] = (float)heightValue[i] * m_info.HeightScale + m_info.HeightOffset;


	}

	std::vector<float> smoothedMap(m_heightMap.size());

	for (unsigned int row = 0; row < m_info.Width; row++)
	{
		for (unsigned int col = 0; col < m_info.Depth; col++)
		{

			smoothedMap[row * m_info.Depth + col] = Average(row, col);

		}
	}
	m_heightMap = smoothedMap;

	return true;
}

GLuint Terrain::CreateTexture(std::string _filePath, std::string _fileName)
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

float Terrain::Average(unsigned int _row, unsigned int _col)
{
	float avg = 0.0f;
	unsigned int count = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int row = _row + i;
			int col = _col + j;

			if (row >= 0 && row < m_info.Width && col >= 0 && col < m_info.Depth)
			{
				avg += m_heightMap[row * m_info.Depth + col];
				count++;
			}
		}
	}

	return count > 0 ? avg / count : 0.0f;
}
