#include "Mesh.h"

void Mesh::Render()
{
	glBindVertexArray(m_VAO);
	if (m_arrayDraw)
	{
		glDrawArrays(m_drawType, 0, m_indexCount);
	}
	else
	{
		glDrawElements(m_drawType, m_indexCount, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);



}

void Mesh::Update()
{

}

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
}
