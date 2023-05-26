#include "MeshModel.h"

MeshModel::MeshModel(std::string _folderPath, std::string _fileName)
{
	//Variable setup
	std::vector<VertexFormat> Vertices;
	std::vector<GLuint> Textures;

	std::string FilePath = _folderPath + _fileName;

	//Path
	tinyobj::ObjReaderConfig ReaderConfig;
	ReaderConfig.mtl_search_path = "";

	tinyobj::ObjReader Reader;

	//Test for incorrect paths
	if (!Reader.ParseFromFile(FilePath, ReaderConfig))
	{
		if (!Reader.Error().empty())
		{
			std::cerr << "TinyObjReader: " << Reader.Error();
		}
		exit(1);
	}

	if (!Reader.Warning().empty()) 
	{
		std::cout << "TinyObjReader: " << Reader.Warning();
	}

	//Setup feature variables for quick access
	auto& Attrib = Reader.GetAttrib();
	auto& Shapes = Reader.GetShapes();
	auto& Materials = Reader.GetMaterials();

	//Loop through the shapes
	for (size_t ShapeIndex = 0; ShapeIndex < Shapes.size(); ShapeIndex++) 
	{
		//Loop through faces of the shape
		size_t IndexOffset = 0;
		for (size_t FaceIndex = 0; FaceIndex < Shapes[ShapeIndex].mesh.num_face_vertices.size(); FaceIndex++)
		{
			size_t FaceVertexCount = size_t(Shapes[ShapeIndex].mesh.num_face_vertices[FaceIndex]);
			//Loop through individual vertices on the face
			for (size_t VertexIndex = 0; VertexIndex < FaceVertexCount; VertexIndex++)
			{
				VertexFormat Vertex{};
				tinyobj::index_t TinyObjVertex = Shapes[ShapeIndex].mesh.indices[IndexOffset + VertexIndex];
				Vertex.Position = {
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 0],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 1],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 2],
				};
				if (TinyObjVertex.normal_index >= 0)//Negative states no Normal data
				{
					Vertex.Normal = {
					Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 0],
					Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 1],
					Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 2],
					};
				}
				if (TinyObjVertex.texcoord_index >= 0)//Negative states no TexCoord data
				{
					Vertex.TexCoord = {
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 0],
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 1],
					};
				}
				Vertices.push_back(Vertex);
			}
			IndexOffset += FaceVertexCount;

			GLuint tex = Shapes[ShapeIndex].mesh.material_ids[FaceIndex];
		}
	}

	m_indexCount = Vertices.size();
	std::cout << "Vertex Size: " << m_indexCount << std::endl;

	GLuint VBO;
	//Bind VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	//Bind VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::Position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::TexCoord)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::Normal)));
	glEnableVertexAttribArray(2);

	m_drawType = GL_TRIANGLES;
	m_arrayDraw = true;
}
