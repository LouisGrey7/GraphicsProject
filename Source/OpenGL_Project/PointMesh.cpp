#include "PointMesh.h"

PointMesh::PointMesh()
{
	std::vector<VertexPoint> Vertices;
	Vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	Vertices.push_back(glm::vec3(200.0f, 0.0f, 0.0f));
	Vertices.push_back(glm::vec3(400.0f, 0.0f, 0.0f));
	Vertices.push_back(glm::vec3(600.0f, 0.0f, 0.0f));
	Vertices.push_back(glm::vec3(800.0f, 0.0f, 0.0f));




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


	m_indexCount = Vertices.size();
	m_drawType = GL_POINTS;
	m_arrayDraw = true;

}

PointMesh::~PointMesh()
{
}
