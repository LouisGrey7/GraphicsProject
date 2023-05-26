#include "Sphere.h"

Sphere::Sphere(float _radius, int _fidelity)
{
	int vertexAttrib = 8;	// Float components are needed for each vertex point
	int indexPerQuad = 6;	// Indices needed to create a quad

	// Angles to keep track of the sphere points 
	float phi = 0.0f;
	float theta = 0.0f;

	// Create the vertex array to hold the correct number of elements based on the fidelity of the sphere
	int vertexCount = _fidelity * _fidelity * vertexAttrib;
	GLfloat* vertices = new GLfloat[vertexCount];
	int element = 0;		

	// Each cycle moves down on the vertical (Y axis) to start the next ring
	for (int i = 0; i < _fidelity; i++)
	{
		// A new  horizontal ring starts at 0 degrees
		theta = 0.0f;

		// Creates a horizontal ring and adds each new vertex point to the vertex array
		for (int j = 0; j < _fidelity; j++)
		{
			// Calculate the new vertex position point with the new angles
			float x = cos(phi) * sin(theta);
			float y = cos(theta);
			float z = sin(phi) * sin(theta);

			// Set the position of the current vertex point
			vertices[element++] = x * _radius;
			vertices[element++] = y * _radius;
			vertices[element++] = z * _radius;
						   		   
			// Set the texture coordinates of the current vertex point
			vertices[element++] = (float)i / (_fidelity - 1);
			vertices[element++] = 1 - ((float)j / (_fidelity - 1)); // 1 minus in order to flip the direction of 0-1 (0 at the bottom)

			// Set the normal direction of the current vertex point
			vertices[element++] = x;
			vertices[element++] = y;
			vertices[element++] = z;

			// Theta (Y axis) angle is incremented based on the angle created by number of sections
			// As the sphere is built ring by ring, the theta is only needed to do half the circumferance therefore using just PI
			theta += ((float)M_PI / ((float)_fidelity - 1.0f));
		}

		// Phi angle (X and Z axes) is incremented based on the angle created by the number of sections
		// Angle uses 2*PI to get the full circumference as this layer is built as a full ring
		phi += (2.0f * (float)M_PI) / ((float)_fidelity - 1.0f);
	}

	// Create the index array to hold the correct number of elements based on the fidelity of the sphere
	m_indexCount = _fidelity * _fidelity * indexPerQuad;
	GLuint* indices = new GLuint[m_indexCount];

	element = 0;	// Reset the element offset for the new array
	for (int i = 0; i < _fidelity; i++)
	{
		for (int j = 0; j < _fidelity; j++)
		{
			// First triangleObj of the quad
			indices[element++] = (((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity);
			indices[element++] = (i * _fidelity) + (j);
			indices[element++] = (((i + 1) % _fidelity) * _fidelity) + (j);

			// Second triangleObj of the quad
			indices[element++] = (i * _fidelity) + ((j + 1) % _fidelity);
			indices[element++] = (i * _fidelity) + (j);
			indices[element++] = (((i + 1) % _fidelity) * _fidelity) + ((j + 1) % _fidelity);
		}
	}
	
	
	// Create the Vertex Array and associated buffers
	GLuint VBO, EBO;
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,  vertexCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  m_indexCount * sizeof(GLuint), indices, GL_STATIC_DRAW);

	// Vertex Information (Position, Texture Coords and Normals)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	m_drawType = GL_TRIANGLES;
	m_arrayDraw = false;

	// Clean up the used memory
	delete[] vertices;
	delete[] indices;
}

// Destructor
Sphere::~Sphere()
{
}

