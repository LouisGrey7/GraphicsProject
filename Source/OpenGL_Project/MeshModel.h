#pragma once
#include <tiny_obj_loader.h>

#include <string>
#include <vector>
#include <iostream>

#include "Mesh.h"
#include "VertexFormat.h"

class MeshModel : public Mesh
{
public:
	MeshModel(std::string _folderPath, std::string _fileName);
	~MeshModel() {};
};

