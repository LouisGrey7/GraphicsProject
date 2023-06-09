// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : main.cpp
// Description : Runs the program
// Author : Louis
// Mail : Louis.Grey@mds.ac.nz
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <stb_image.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "ShaderLoader.h"
#include "TextLabel.h"
#include "Object.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "PerlinNoise.h"
#include "Quad.h"
#include "DDDTexture.h"
#include "PointMesh.h"
#include "QuadPatch.h"
#include "TrianglePatch.h"
#include "Framebuffer.h"

void InitialSetup();
void Update();
void ProcessInput(float deltaTime);
void Render();

//Variables
GLFWwindow* window = nullptr;

//Delta Time
float previousTimeStep;
float currentTimeStep;
float deltaTime;

//For Mouse Control
double lastX = 0;
double lastY = 0;
bool cursorOn = true;

//Scene Control
bool wireframeMode = false;

// Programs
GLuint program_Texture;
GLuint program_BlinnPhong;
GLuint program_SkyBox;
GLuint program_GeoVertex;
GLuint program_GeoVertexNormals;
GLuint program_GeoVertexExplode;
GLuint program_QuadTess;
GLuint program_TriTess;
GLuint program_Inverse;
GLuint program_Greyscale;
GLuint program_Rain;
GLuint program_CRT;


//Cameras
Camera* mainCamera;

//Light Manager
LightManager* lightManager;
	
//SkyBox
SkyBox* skyBox;

//FRAMEBUFFER
Framebuffer* frameBuffer;

//Meshes
PointMesh* pointMesh;
QuadPatch* quadPatch;
TrianglePatch* trianglePatch;
Quad* quadMesh;
MeshModel* bearMesh;
MeshModel* dragonMesh;
Sphere* sphereMesh;


//Objects
Object* starObj;
Object* triangleObj;
Object* screenQuadObj;
Object* bearObj;
Object* geoBearObj;
Object* explodeBearObj;
Object* dragonObj;
Object* sphereObj;


enum PostProcessing
{
	COLOR,
	INVERSE,
	GREYSCALE,
	RAIN,
	CRT
};

enum CurrentScene
{

	SCENE1,
	SCENE2,
	SCENE3

};

PostProcessing postProcessing = COLOR;
CurrentScene currentScene = SCENE1;

int main()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


	// Create a GLFW Window
	window = glfwCreateWindow(Utils::WindowWidth, Utils::WindowHeight, "OpenGL Assignment 3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "GLFW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initializing GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	// Setup
	InitialSetup();

	//Initialize IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	// Main loop
	while (glfwWindowShouldClose(window) == false)
	{

		Update();

			
		Render();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Shutdown GLFW
	glfwTerminate();
	return 0;
}
	

void InitialSetup()
{
	// Set the color of the window
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Maps the range of the window size
	glViewport(0, 0, Utils::WindowWidth, Utils::WindowHeight);

	// Enable Depth testing for 3D
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//PROGRAMS
	program_Texture = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
												  "Resources/Shaders/Texture.fs");

	program_BlinnPhong = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
													 "Resources/Shaders/3DLight_BlinnPhong.fs");

	program_Inverse = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
												  "Resources/Shaders/InverseColor.fs");

	program_Greyscale = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
												  "Resources/Shaders/Greyscale.fs");

	program_Rain = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
													"Resources/Shaders/Rain.fs");

	program_CRT = ShaderLoader::CreateProgram("Resources/Shaders/3D_Normals.vs",
											   "Resources/Shaders/CRT.fs");

	program_SkyBox = ShaderLoader::CreateProgram("Resources/Shaders/SkyBox.vs",
												 "Resources/Shaders/SkyBox.fs");

	program_GeoVertex = ShaderLoader::CreateProgramVGF("Resources/Shaders/GeoVertex.vs", 
													   "Resources/Shaders/Star.gs",
													   "Resources/Shaders/Texture.fs");

	program_GeoVertexNormals = ShaderLoader::CreateProgramVGF("Resources/Shaders/GeoVertNorm.vs",
															  "Resources/Shaders/ModelGeo.gs",
														      "Resources/Shaders/Outline.fs");

	program_GeoVertexExplode = ShaderLoader::CreateProgramVGF("Resources/Shaders/GeoVertNorm.vs",
															  "Resources/Shaders/ExplodeGeo.gs",
															  "Resources/Shaders/Texture.fs");

	program_QuadTess = ShaderLoader::CreateProgramVTF("Resources/Shaders/PositionOnly.vs",
														"Resources/Shaders/TCS.tcs",
														"Resources/Shaders/TES.tes",
														"Resources/Shaders/Outline.fs");

	program_TriTess = ShaderLoader::CreateProgramVTF("Resources/Shaders/PositionOnly.vs",
													 "Resources/Shaders/TriTCS.tcs",
													 "Resources/Shaders/TriTES.tes",
													 "Resources/Shaders/Outline.fs");

	// DELTATIME
	previousTimeStep = (float)glfwGetTime();

	// CURSOR
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	// CAMERA
	mainCamera = new Camera(PERSPECTIVE);

	//LIGHTMANAGER
	lightManager = new LightManager();
	lightManager->CreatePointLight(0, glm::vec3(0.0f, 50.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 1.0f, 2.0f, 0.025f, 0.0075f);

	//SKYBOX
	skyBox = new SkyBox(mainCamera, program_SkyBox, "Clouds", 0);

	//FRAMEBUFFER
	frameBuffer = new Framebuffer();


	//3D OBJECTS
	pointMesh = new PointMesh();
	starObj = new Object(mainCamera, pointMesh, program_GeoVertex, lightManager, glm::vec3(-800.0f, 0.0f, -2400.0f), true, "Sphere.jpg");


	trianglePatch = new TrianglePatch();
	triangleObj = new Object(mainCamera, trianglePatch, program_TriTess, lightManager, glm::vec3(0.0f, 0.0f, -20.0f), true, "Sphere.jpg");

	quadMesh = new Quad();
	screenQuadObj = new Object(mainCamera, quadMesh, program_Greyscale, lightManager, glm::vec3(0.0f, 0.0f, -20.0f), true, frameBuffer->GetTexture());

	bearMesh = new MeshModel("Resources/Models/Bear/", "Bear.obj");
	bearObj = new Object(mainCamera, lightManager, program_BlinnPhong, glm::vec3(0.0f, -5.0f, -140.0f), "Resources/Models/Bear/", "Bear.obj", "Bear.png", 0);
	geoBearObj = new Object(mainCamera, lightManager, program_GeoVertexNormals, glm::vec3(12.0f, 5.0f, -70.0f), "Resources/Models/Bear/", "Bear.obj", "Bear.png", 0);
	explodeBearObj = new Object(mainCamera, lightManager, program_GeoVertexExplode, glm::vec3(-12.0f, 5.0f, -70.0f), "Resources/Models/Bear/", "Bear.obj", "Bear.png", 0);

	dragonMesh = new MeshModel("Resources/Models/Dragon/", "Dragon.obj");
	dragonObj = new Object(mainCamera, lightManager, program_BlinnPhong, glm::vec3(-30.0f, -5.0f, -140.0f), "Resources/Models/Dragon/", "Dragon.obj", "Dragon.png", 0);

	sphereMesh = new Sphere(3.0f, 100.0f);
	sphereObj = new Object(mainCamera, sphereMesh, program_BlinnPhong, lightManager, glm::vec3(30.0f, -5.0f, -140.0f), true, "Sphere.jpg");


	glfwGetCursorPos(window, &lastX, &lastY);
}

	
void Update()
{
	// Calculate DELTATIME
	currentTimeStep = (float)glfwGetTime();
	deltaTime = currentTimeStep - previousTimeStep;
	previousTimeStep = currentTimeStep;

	glfwPollEvents();

	//CAMERA MOVE
	//ProcessInput(deltaTime);

	mainCamera->Update(deltaTime);

	starObj->Update(deltaTime);

	triangleObj->Update(deltaTime);


	screenQuadObj->Update(deltaTime);

	skyBox->Update(deltaTime);
	bearObj->Update(deltaTime);
	geoBearObj->Update(deltaTime);
	explodeBearObj->Update(deltaTime);
	dragonObj->Update(deltaTime);
	sphereObj->Update(deltaTime);

	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		
		if (postProcessing == COLOR)
		{
			postProcessing = INVERSE;
		}
		else if (postProcessing == INVERSE)
		{
			postProcessing = GREYSCALE;
		}
		else if (postProcessing == GREYSCALE)
		{
			postProcessing = RAIN;
		}
		else if (postProcessing == RAIN)
		{
			postProcessing = CRT;
		}
		else if (postProcessing == CRT)
		{
			postProcessing = COLOR;
		}
	}
}

	
void ProcessInput(float deltaTime)
{
	double Xpos;
	double Ypos;

	float CameraSpeed = 20.0f * deltaTime;
	float Sensitivity = 0.1f;

	//Mouse Controls
	glfwGetCursorPos(window, &Xpos, &Ypos);
	float xOffset = Xpos - lastX;
	float yOffset = lastY - Ypos;

	lastX = Xpos;
	lastY = Ypos;

	xOffset *= Sensitivity;
	yOffset *= Sensitivity;

	mainCamera->m_Yaw += xOffset;
	mainCamera->m_Pitch += yOffset;

	if (mainCamera->m_Pitch > 89.0f)
	{
		mainCamera->m_Pitch = 89.0f;
	}

	if (mainCamera->m_Pitch < -89.0f)
	{
		mainCamera->m_Pitch = -89.0f;
	}

	mainCamera->CameraLook();




	static int oldState = GLFW_RELEASE;
	int newState = glfwGetKey(window, GLFW_KEY_C);
	//Show Cursor
	if (newState == GLFW_PRESS && oldState == GLFW_PRESS)
	{
		if (cursorOn)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cursorOn = false;
		}
		else
		{

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursorOn = true;

		}


	}
	oldState = newState;


	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//std::cout << "Mouse button left: Press" << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		mainCamera->m_Position += CameraSpeed * mainCamera->m_CameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		mainCamera->m_Position -= CameraSpeed * mainCamera->m_CameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		mainCamera->m_Position -= glm::normalize(glm::cross(mainCamera->m_CameraFront, mainCamera->m_UpDirection)) * CameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		mainCamera->m_Position += glm::normalize(glm::cross(mainCamera->m_CameraFront, mainCamera->m_UpDirection)) * CameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mainCamera->m_Position += mainCamera->m_UpDirection * CameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		mainCamera->m_Position -= mainCamera->m_UpDirection * CameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		exit(0);
	}

}


void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	switch (currentScene)
	{
	case SCENE1:
		skyBox->Render();

		starObj->Render();

		geoBearObj->SetProgram(program_Texture);
		geoBearObj->Render();
		geoBearObj->SetProgram(program_GeoVertexNormals);
		geoBearObj->Render();

		explodeBearObj->Render();

		break;
	case SCENE2:
		skyBox->Render();

		triangleObj->Render();
		break;
	case SCENE3:

		switch (postProcessing)
		{
		case COLOR:
			screenQuadObj->SetProgram(program_Texture);
			break;
		case INVERSE:
			screenQuadObj->SetProgram(program_Inverse);
			break;
		case GREYSCALE:
			screenQuadObj->SetProgram(program_Greyscale);
			break;
		case RAIN:
			screenQuadObj->SetProgram(program_Rain);
			break;
		case CRT:
			screenQuadObj->SetProgram(program_CRT);
			break;
		default:
			break;
		}

		frameBuffer->Bind();

		skyBox->Render();
		bearObj->Render();
		dragonObj->Render();
		sphereObj->Render();

		frameBuffer->Unbind();

		screenQuadObj->Render();
		break;
	default:
		break;
	}

	//ImGui Scene Control
	ImGui::Begin("Control Menu");
	if (ImGui::Button("Scene 1") || glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		currentScene = SCENE1;
	}
	if (ImGui::Button("Scene 2") || glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{

		currentScene = SCENE2;

	}
	if (ImGui::Button("Scene 3") || glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		currentScene = SCENE3;
	}
	if (ImGui::Button("Color"))
	{
		postProcessing = COLOR;
	}
	if (ImGui::Button("Inverse"))
	{
		postProcessing = INVERSE;
	}
	if (ImGui::Button("Greyscale"))
	{
		postProcessing = GREYSCALE;
	}
	if (ImGui::Button("Rain"))
	{
		postProcessing = RAIN;
	}
	if (ImGui::Button("CRT"))
	{
		postProcessing = CRT;
	}


	ImGui::Checkbox("Wireframe Mode", &wireframeMode);

	if (wireframeMode || glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}


	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

	






