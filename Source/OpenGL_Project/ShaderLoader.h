#pragma once
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include<iostream>
#include<fstream>
#include<vector>

class ShaderLoader
{
private:

	static GLuint CreateShader(GLenum _shaderType, const char* _shaderName);
	static std::string ReadShaderFile(const char *_filename);
	static void PrintErrorDetails(bool _isShader, GLuint _id, const char* _name);
	
public:
	static GLuint CreateProgram(const char* _vertexShaderFilename, const char* _fragmentShaderFilename);
	static GLuint CreateProgramVGF(const char* _vertexShaderFilename, const char* _geometryShaderFileName, const char* _fragmentShaderFilename);
	GLuint CreateProgramVTF(const char* _vertexShaderFilename, const char* _TCSShaderFileName, const char* _TESShaderFileName, const char* _fragmentShaderFilename);
};

