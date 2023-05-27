

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




// Variables
GLFWwindow* window = nullptr;

// Delta Time
float previousTimeStep;
float currentTimeStep;
float deltaTime;

// For Mouse Control
double lastX = 0;
double lastY = 0;
bool cursorOn = true;

//Scene Control
bool scene1On = false;
bool scene2On = false;
bool scene3On = false;
bool wireframeMode = true;

// Programs
GLuint program_Texture;
GLuint program_BlinnPhong;
GLuint program_SkyBox;
GLuint program_GeoVertex;
GLuint program_QuadTess;
GLuint program_TriTess;


// Cameras
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

//Objects
Object* starObj;
Object* quadObj;
Object* triangleObj;
Object* screenQuadObj;
Object* bearObj;




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

	// Enable Culling for 3D efficiency


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

	program_SkyBox = ShaderLoader::CreateProgram("Resources/Shaders/SkyBox.vs",
												 "Resources/Shaders/SkyBox.fs");

	program_GeoVertex = ShaderLoader::CreateProgramVGF("Resources/Shaders/GeoVertex.vs", 
													   "Resources/Shaders/Star.gs",
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

	// Keep the default cursor visibility
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	// CAMERA
	mainCamera = new Camera(PERSPECTIVE);

	//LIGHTMANAGER
	lightManager = new LightManager();
	lightManager->CreatePointLight(0, glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.93f, 0.61f, 1.0f), 0.1f, 2.0f, 1.0f, 0.025f, 0.0075f);

	//SKYBOX
	skyBox = new SkyBox(mainCamera, program_SkyBox, "Clouds", 0);

	//FRAMEBUFFER
	frameBuffer = new Framebuffer();


	//3D OBJECTS
	pointMesh = new PointMesh();
	starObj = new Object(mainCamera, pointMesh, program_GeoVertex, lightManager, glm::vec3(-800.0f, 0.0f, -2000.0f), true, "Sphere.jpg");

	quadPatch = new QuadPatch();
	quadObj = new Object(mainCamera, quadPatch, program_QuadTess, lightManager, glm::vec3(0.0f, 0.0f, 0.0f), true, "Sphere.jpg");

	trianglePatch = new TrianglePatch();
	triangleObj = new Object(mainCamera, trianglePatch, program_TriTess, lightManager, glm::vec3(0.0f, 0.0f, -20.0f), true, "Sphere.jpg");

	quadMesh = new Quad();
	screenQuadObj = new Object(mainCamera, quadMesh, program_BlinnPhong, lightManager, glm::vec3(0.0f, 0.0f, -30.0f), true, frameBuffer->GetTexture());

	bearMesh = new MeshModel("Resources/Models/Bear/", "Bear.obj");
	bearObj = new Object(mainCamera, lightManager, program_BlinnPhong, glm::vec3(0.0f, 0.0f, -100.0f), "Resources/Models/Bear/", "Bear.obj", "Bear.png", 0);

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



	
		float rotation = 0;
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//SCENE 1
	if (scene1On)
	{
		skyBox->Render();

		starObj->Render();

	}

	//SCENE 2
	if (scene2On)
	{
		skyBox->Render();

		triangleObj->Render();

	}

	//SCENE 3
	if (scene3On)
	{
		frameBuffer->Bind();

		skyBox->Render();
		bearObj->Render();

		frameBuffer->Unbind();


		screenQuadObj->Render();
	}


	//ImGui Scene Control
	ImGui::Begin("Control Menu");
	if (ImGui::Button("Scene 1") || glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		scene1On = false;
		scene2On = false;
		scene3On = false;


		if (scene1On == false)
		{
			scene1On = true;
		}
		else
		{

			scene1On = false;

		}
	}
	if (ImGui::Button("Scene 2") || glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		scene1On = false;
		scene2On = false;
		scene3On = false;


		if (scene2On == false)
		{
			scene2On = true;
		}
		else
		{

			scene2On = false;

		}
	}
	if (ImGui::Button("Scene 3") || glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		scene1On = false;
		scene2On = false;
		scene3On = false;


		if (scene3On == false)
		{
			scene3On = true;
		}
		else
		{

			scene3On = false;

		}
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
	
	ImGui::Text("C = Toggle Cursor");
	ImGui::Text("WASDQE = Camera Movement");


	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

	






