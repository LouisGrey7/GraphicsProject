#include "TrianglePatch.h"

TrianglePatch::TrianglePatch()
{
	std::vector<VertexPoint> Vertices;
	Vertices.push_back(glm::vec3(glm::sin(glm::radians(0.0f)), glm::cos(glm::radians(0.0f)), 0.0f));
	Vertices.push_back(glm::vec3(glm::sin(glm::radians(240.0f)), glm::cos(glm::radians(240.0f)), 0.0f));
	Vertices.push_back(glm::vec3(glm::sin(glm::radians(120.0f)), glm::cos(glm::radians(120.0f)), 0.0f));


	glPatchParameteri(GL_PATCH_VERTICES, 3);
	m_indexCount = (unsigned int)Vertices.size();
	m_drawType = GL_PATCHES;
	m_arrayDraw = true;

	GLuint VBO;
	//Bind VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	//Bind VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPoint) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPoint), (void*)(offsetof(VertexPoint, VertexPoint::Position)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



}

TrianglePatch::~TrianglePatch()
{
}