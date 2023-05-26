#include "ShaderLoader.h" 


GLuint ShaderLoader::CreateShader(GLenum _shaderType, const char* _shaderName)
{
	// Read the shader files and save the source code as strings
	std::string shaderSourceCode = ReadShaderFile(_shaderName);

	// Create the shader ID and create pointers for source code string and length
	GLuint shaderID = glCreateShader(_shaderType);
	const char *shader_code_ptr = shaderSourceCode.c_str();
	const int shader_code_size = shaderSourceCode.size();

	// Populate the Shader Object (ID) and compile
	glShaderSource(shaderID, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shaderID);

	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails(true, shaderID, _shaderName);
		return 0;
	}
	return shaderID;
}

std::string ShaderLoader::ReadShaderFile(const char *_filename)
{
	// Open the file for reading
	std::ifstream file(_filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << _filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);

	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

void ShaderLoader::PrintErrorDetails(bool _isShader, GLuint _id, const char* _name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(_isShader == true) ? glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(_isShader == true) ? glGetShaderInfoLog(_id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(_id, infoLogLength, NULL, &log[0]);		
	std::cout << "Error compiling " << ((_isShader == true) ? "shader" : "program") << ": " << _name << std::endl;
	std::cout << &log[0] << std::endl;
}

GLuint ShaderLoader::CreateProgram(const char* _vertexShaderFilename, const char* _fragmentShaderFilename)
{
	// Create the shaders from the filepath
	GLuint vertexShaderID = CreateShader(GL_VERTEX_SHADER, _vertexShaderFilename);
	GLuint fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, _fragmentShaderFilename);
	
	// Create the program handle, attach the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShaderID);
	glAttachShader(program, fragmentShaderID);
	glLinkProgram(program);

	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = _vertexShaderFilename + *_fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}

GLuint ShaderLoader::CreateProgramVGF(const char* _vertexShaderFilename, const char* _geometryShaderFileName, const char* _fragmentShaderFilename)
{
	// Create the shaders from the filepath
	GLuint vertexShaderID = CreateShader(GL_VERTEX_SHADER, _vertexShaderFilename);
	GLuint geometryShaderID = CreateShader(GL_GEOMETRY_SHADER, _geometryShaderFileName);
	GLuint fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, _fragmentShaderFilename);


	// Create the program handle, attach the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShaderID);
	glAttachShader(program, geometryShaderID);
	glAttachShader(program, fragmentShaderID);
	glLinkProgram(program);

	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = _vertexShaderFilename + *_geometryShaderFileName + *_fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}

GLuint ShaderLoader::CreateProgramVTF(const char* _vertexShaderFilename, const char* _TCSShaderFileName, const char* _TESShaderFileName, const char* _fragmentShaderFilename)
{
	// Create the shaders from the filepath
	GLuint vertexShaderID = CreateShader(GL_VERTEX_SHADER, _vertexShaderFilename);
	GLuint TCSShaderID = CreateShader(GL_TESS_CONTROL_SHADER, _TCSShaderFileName);
	GLuint TESShaderID = CreateShader(GL_TESS_EVALUATION_SHADER, _TESShaderFileName);
	GLuint fragmentShaderID = CreateShader(GL_FRAGMENT_SHADER, _fragmentShaderFilename);


	// Create the program handle, attach the shaders and link it
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShaderID);
	glAttachShader(program, TCSShaderID);
	glAttachShader(program, TESShaderID);
	glAttachShader(program, fragmentShaderID);
	glLinkProgram(program);

	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = _vertexShaderFilename + *_TCSShaderFileName + *_TESShaderFileName + *_fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}