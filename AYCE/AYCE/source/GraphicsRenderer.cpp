/*****************************************************************************/
/*!
\file   GraphicsRenderer.cpp
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for GraphicsRenderer features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "precompiled.h"
#include "GraphicsRenderer.h"
#include "Application.h"
#include "Component.h"
//#include "ShaderProgram.h"
#include "InputSystem.h"
#include "EntitySystem.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Message.h"
#include "ModelNode.h"
#include "LightNode.h"
#include "ImguiManager.h"
#include "Imgui.h"
#include "Physics.h"
#include "MeshComponent.h"
#include "Geometry.h"
#include "StateSystem.h"

#include "Application.h"

//#define MPR_COLLISION_ON 
//#define DVBH_COLLISION_ON
namespace VK
{
	// Global static pointer used to ensure a single instance of the class.
	GraphicsRenderer* GraphicsRenderer::m_pInstance = nullptr;

	static GLuint quadVAO = 0;
	static GLuint quadVBO;

	GraphicsRenderer::GraphicsRenderer() :
		m_fullscreen(false),
		m_windowWidth(0),
		m_windowHeight(0),
		objnumber(0),
		shaderGeometryPass(nullptr),
		shaderLightingPass(nullptr),
		shaderLightBox(nullptr)
	{
		m_pStateSystem = nullptr;
		
		m_app = Application::GetAppInstance();

		m_nRenderMode = 0;

		dirColor = glm::vec3(1.f);
		LightDirction = glm::vec3(0.f, 0.f, -1.f);
		/*
		if (!gbuffer)
		gbuffer = new GBuffer();
		*/
		//m_pCamera = new Camera(glm::vec3(0.0f, 50.0f, 50.0f));
	}
	// This function loads a texture from file. Note: texture loading functions like these are usually 
	// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
	// For learning purposes we'll just define it as a utility function.
	GLuint loadTexture(GLchar* path, GLboolean alpha)
	{
		//Generate texture ID and load texture data 
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height;
		unsigned char* image = SOIL_load_image(path, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, alpha ? GL_RGBA : GL_RGB, width, height, 0, alpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);	// Use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes value from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, alpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
		return textureID;

	}

	GraphicsRenderer::~GraphicsRenderer()
	{
		glDeleteBuffers(1, &gBuffer);
		glDeleteBuffers(1, &gPosition);
		glDeleteBuffers(1, &gNormal);
		glDeleteBuffers(1, &gAlbedoSpec);
		glDeleteBuffers(1, &rboDepth);
		glDeleteBuffers(1, &planeVBO);
		glDeleteBuffers(1, &transparentVBO);
		glDeleteVertexArrays(1, &planeVAO);
		glDeleteVertexArrays(1, &transparentVAO);
		
		glDeleteBuffers(1, &quadVBO);
		glDeleteVertexArrays(1, &quadVAO);

		quadVAO = 0;
		
		SAFE_DELETE(m_pInstance);
	}

	GraphicsRenderer* GraphicsRenderer::GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new GraphicsRenderer();

		return m_pInstance;
	}

	void GraphicsRenderer::setSize(int w, int h)
	{
		m_windowWidth = w;
		m_windowHeight = h;

		glViewport(0, 0, m_windowWidth, m_windowHeight);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//gluPerspective(40.0f, static_cast<GLfloat>(m_windowWidth) / static_cast<GLfloat>(m_windowHeight), 1.0f, 30.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void GraphicsRenderer::ProcessMessage(Message* message)
	{

	}

	//GLuint loadTexture(GLchar const * path)
	//{
	//	// Generate texture ID and load texture data 
	//	GLuint textureID;
	//	glGenTextures(1, &textureID);
	//	int width, height;
	//	unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	//	// Assign texture to ID
	//	glBindTexture(GL_TEXTURE_2D, textureID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//	glGenerateMipmap(GL_TEXTURE_2D);

	//	// Parameters
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	////	SOIL_free_image_data(image);
	//	return textureID;

	//}
	// RenderCube() Renders a 1x1 3D cube in NDC.
	GLuint cubeVAO = 0;
	GLuint cubeVBO = 0;
		GLuint transparentTexture;
	void RenderCube()
	{
		// Initialize (if necessary)
		if (cubeVAO == 0)
		{
			GLfloat vertices[] = {
				// Back face
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
				0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
				-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
																  // Front face
																  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
																  0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
																  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
																  0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
																  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
																  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
																													 // Left face
																													 -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
																													 -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
																													 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
																													 -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
																													 -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
																													 -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
																																									   // Right face
																																									   0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
																																									   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
																																									   0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
																																									   0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
																																									   0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
																																									   0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
																																																						// Bottom face
																																																						-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
																																																						0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
																																																						0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
																																																						0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
																																																						-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
																																																						-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
																																																																			// Top face
																																																																			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
																																																																			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
																																																																			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
																																																																			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
																																																																			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
																																																																			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
			};
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);
			// Fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// Link vertex attributes
			glBindVertexArray(cubeVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// Render Cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	//-----------------------------------------------------------------------------
	// Name: init()
	// Desc: 
	//-----------------------------------------------------------------------------
	void GraphicsRenderer::Init()
	{
		static bool once = true;
		START();

		GLFWmonitor * monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		//glfwSetWindowMonitor(m_window, monitor, 0,0,mode->width, mode->height, mode->refreshRate);

		m_windowWidth = 1920;// mode->width;
		m_windowHeight = 1080;// mode->height;
		
		m_app = Application::GetAppInstance();
		m_pStateSystem = m_app->getSystem<StateSystem>();

		//m_lightRotation = 0.0f;

		//glDisable(GL_LIGHTING);
		//glEnable(GL_TEXTURE_2D);
		//glShadeModel(GL_SMOOTH);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		//glEnable(GL_CULL_FACE);

		//GLenum err = glewInit();
		//if (GLEW_OK != err)
		//	return;

		//loadAssets();


		
		// Setup and compile our shaders
		if(!shaderGeometryPass)
			shaderGeometryPass = new Shader("shaders/g_buffer.vs", "shaders/g_buffer.frag");
		if(!shaderLightingPass)
			shaderLightingPass = new Shader("shaders/deferred_shading.vs", "shaders/deferred_shading.frag");
		if(!shaderLightBox)
			shaderLightBox = new Shader("shaders/deferred_light_box.vs", "shaders/deferred_light_box.frag");
		/*
		if(!shaderUI)
			shaderUI = new Shader("shaders/blending_sorted.vs", "shaders/blending_sorted.frag");
	
		shaderUI->Use();
		glUniform1i(glGetUniformLocation(shaderLightingPass->Program, "gPosition"), 0);
		*/

		// Set samplers
		shaderLightingPass->Use();
		glUniform1i(glGetUniformLocation(shaderLightingPass->Program, "gPosition"), 0);
		glUniform1i(glGetUniformLocation(shaderLightingPass->Program, "gNormal"), 1);
		glUniform1i(glGetUniformLocation(shaderLightingPass->Program, "gAlbedoSpec"), 2);

		

		//if (once)
		{
			glGenFramebuffers(1, &gBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

			// - Position color buffer
			glGenTextures(1, &gPosition);
			glBindTexture(GL_TEXTURE_2D, gPosition);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_windowWidth, m_windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
			// - Normal color buffer
			glGenTextures(1, &gNormal);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_windowWidth, m_windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
			// - Color + Specular color buffer
			glGenTextures(1, &gAlbedoSpec);
			glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_windowWidth, m_windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
			// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
			GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);
			// - Create and attach depth buffer (renderbuffer)

			glGenRenderbuffers(1, &rboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_windowWidth, m_windowHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
			// - Finally check if framebuffer is complete
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


			GLfloat planeVertices[] = {
				// Positions          // Texture Coords (note we set these higher than 1 that together with GL_REPEAT as texture wrapping mode will cause the floor texture to repeat)
				5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
				-5.0f, -0.5f,  5.0f,  0.0f,  0.0f,
				-5.0f, -0.5f, -5.0f,  0.0f,  2.0f,

				5.0f, -0.5f,  5.0f,  2.0f,  0.0f,
				-5.0f, -0.5f, -5.0f,  0.0f,  2.0f,
				5.0f, -0.5f, -5.0f,  2.0f,  2.0f
			};
			GLfloat transparentVertices[] = {
				// Positions         // Texture Coords (swapped y coordinates because texture is flipped upside down)
				0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
				0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
				1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

				0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
				1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
				1.0f,  0.5f,  0.0f,  1.0f,  0.0f
			};

			// Setup plane VAO

			glGenVertexArrays(1, &planeVAO);
			glGenBuffers(1, &planeVBO);
			glBindVertexArray(planeVAO);
			glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glBindVertexArray(0);
			// Setup transparent plane VAO

			glGenVertexArrays(1, &transparentVAO);
			glGenBuffers(1, &transparentVBO);
			glBindVertexArray(transparentVAO);
			glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glBindVertexArray(0);

			//transparentTexture = loadTexture("models/window.png", true);
			// - Colors
			//const GLuint NR_LIGHTS = 1;

			////srand(13);
			//for (GLuint i = 0; i < NR_LIGHTS; i++)
			//{
			//	// Calculate slightly random offsets
			//	GLfloat xPos = static_cast<GLfloat>(((rand() % 1000) / 1000.0f) * 1000.0f);
			//	GLfloat yPos = static_cast<GLfloat>(((rand() % 1000) / 1000.0f) * 100.0f + 10.f);
			//	GLfloat zPos = static_cast<GLfloat>(((rand() % 1000) / 1000.0f) * 1000.0f);
			//	lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
			//	// Also calculate random color
			//	GLfloat rColor = static_cast<GLfloat>(((rand() % 100) / 200.0f) + 0.5); // Between 0.5 and 1.0
			//	GLfloat gColor = static_cast<GLfloat>(((rand() % 100) / 200.0f) + 0.5); // Between 0.5 and 1.0
			//	GLfloat bColor = static_cast<GLfloat>(((rand() % 100) / 200.0f) + 0.5); // Between 0.5 and 1.0
			//	lightColors.push_back(glm::vec3(rColor, gColor, bColor));
			//}


			//cyborg = new Model("models/cube.obj", false);
			//initial number of lights
			//int nmLights = 1;
			//srand(unsigned int(time(nullptr)));
			//meshtypes.emplace("sphere", new Mesh("models/sphere.obj"));

			//for (int i = 0; i <= nmLights; ++i)
			//{
			//	std::string pointlight = "pointlight" + std::to_string(i);
			//	//lights.emplace("pointlight", new LightNode(30.0f, rand() % 255, rand() % 255, rand() % 255));
			//	lights.emplace(pointlight.c_str(), new LightNode(30.0f, static_cast<float>(rand() % 255), static_cast<float>(rand() % 255), static_cast<float>(rand() % 255)));
			//	lights[pointlight.c_str()]->direction = glm::vec3((rand() % 2) - 0.5f, 0.0f, (rand() % 2) - 0.5f);
			//	lights[pointlight.c_str()]->getTransform().setTranslation(rand() % 100 - 50.f, 7.0f, rand() % 100 - 50.f);
			//	lights[pointlight.c_str()]->setMesh(meshtypes["sphere"]);

			//	++i;
			//}
			if (once)
			{
				objnumber = 0;
				once = false;
			}
		}
		//once = false;

		LOG("Init OpenGL");
	}
	
	
	void RenderQuad()
	{
		if (quadVAO == 0)
		{
			GLfloat quadVertices[] = {
				// Positions        // Texture Coords
				-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// Setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	
	void GraphicsRenderer::Render()
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		////glClearColor(1.0f, 0.3f, 0.8f, 1.0f);

		//// We move the near plane just a bit to make the depth texture a bit more visible. 
		//// It also increases the precision.
		//glMatrixMode(GL_PROJECTION);

		//glPushMatrix();
		//glLoadIdentity();
		////gluPerspective(20.0f, 1, 40.0f, 70.0f);

		//// Set the light position
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glRotatef(55, 1, 0, 0);
		//glRotatef(-45, 0, 1, 0);
		//glTranslatef(-25.0f, -50.0f, -25.0f);
		//glRotatef(m_lightRotation, 0, 1, 0);
		//glFrontFace(GL_CW);

		//// Render the shadow map
		//m_deferredRendering->startRenderToShadowMap();
		//for (auto it = m_models.begin(); it != m_models.end(); ++it)
		//	it->second->render();
		//m_deferredRendering->stopRenderToShadowMap();

		//// We then save out the matrices and send them to the deferred rendering, so when it comes to do the deferred pass
		//// it can project the pixel it's rendering to the light and see if it's in shadows
		//float worldToLightViewMatrix[16];
		//float lightViewToProjectionMatrix[16];
		//glGetFloatv(GL_MODELVIEW_MATRIX, worldToLightViewMatrix);
		//glGetFloatv(GL_PROJECTION_MATRIX, lightViewToProjectionMatrix);

		//// Re-set the projection to the default one we have pushed on the stack
		//glMatrixMode(GL_PROJECTION);
		//glPopMatrix();

		//// Set the camera position
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		//glRotatef(20, 1, 0, 0);
		//glTranslatef(0.0f, -6.5f, -11.0f);
		//glFrontFace(GL_CCW);

		//float worldToCameraViewMatrix[16];
		//glGetFloatv(GL_MODELVIEW_MATRIX, worldToCameraViewMatrix);

		//// Render our geometry into the FBO
		//m_deferredRendering->startRenderToFBO();
		//
		//for (auto it = m_models.begin(); it != m_models.end(); ++it)
		//	it->second->render();
		//m_deferredRendering->stopRenderToFBO();

		//// Render to the screen
		//
		//// Render to screen using the deferred rendering shader
		//m_deferredRendering->setLightMatrices(worldToLightViewMatrix, lightViewToProjectionMatrix, worldToCameraViewMatrix);
		//m_deferredRendering->render();
		
		
		/************************************************************************/
		// Clear the colorbuffer
		//glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		


		//glViewport(0, 0, 1280, 960);
		//GBuffer& gbuffer = VK::ShaderManager::GetInstance()->getGbuffer();

		// Update the camera matrices
		VK::Camera::GetInstance()->UpdateCamera();
		//gbuffer->startFrame();

		int windowWidth, windowHeight;
		glfwGetWindowSize(m_app->GetWindow(), &windowWidth, &windowHeight);
		//windowWidth = 1920;
		//windowHeight = 1080;
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glStencilFunc(GL_ALWAYS, 0, 0x00);

		// 1. Geometry Pass: render scene's geometry/color data into gbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 projection = VK::Camera::GetInstance()->GetProjectionMatrix();
		glm::mat4 view = VK::Camera::GetInstance()->GetViewMatrix();
		glm::mat4 model;

		shaderGeometryPass->Use();
		glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


		



		VK::EntitySystem* pES = m_app->getSystem<VK::EntitySystem>();
		VK::EntityListVector e_dynamiclist = pES->GetDynamicEntityList();
		VK::EntityListVector e_staticlist = pES->GetStaticEntityList();
		VK::EntityListVector e_debuglist = pES->GetDebugEntityList();

		/*for (GLuint i = 0; i < objectPositions.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, objectPositions[i]);
			model = glm::scale(model, glm::vec3(1.f));
			glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			cyborg->Draw(*shaderGeometryPass);
		}*/
		//GLfloat angle = (float)glfwGetTime() * 10.f;
		//model = glm::mat4();
		//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		//model = glm::scale(model, glm::vec3(100.f));
		////model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//cyborg->Draw(*shaderGeometryPass);

		//glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		
		if (m_pStateSystem->GetCurrentState() == VK::STATE::STATE_PAUSE || m_pStateSystem->GetCurrentState() == VK::STATE::STATE_HOWTOPLAY ||
			m_pStateSystem->GetCurrentState() == VK::STATE::STATE_OPTION)
		{
			for (auto it : e_debuglist)
			{
				model = it->FindComponent<TransformComponent>()->getTransformation();
				glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				MeshComponent* mesh = it->FindComponent<MeshComponent>();
				if (mesh)
					mesh->getModel()->Draw(*shaderGeometryPass);
			}
		}
		else
		{
			for (auto it : e_dynamiclist)
			{
				//model = glm::mat4();
				model = it->FindComponent<TransformComponent>()->getTransformation();
				glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				MeshComponent* mesh = it->FindComponent<MeshComponent>();
				if(mesh)
					mesh->getModel()->Draw(*shaderGeometryPass);
			}

			for (auto it : e_staticlist)
			{
				model = it->FindComponent<TransformComponent>()->getTransformation();
				glUniformMatrix4fv(glGetUniformLocation(shaderGeometryPass->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
				MeshComponent* mesh = it->FindComponent<MeshComponent>();
				if (mesh)
					mesh->getModel()->Draw(*shaderGeometryPass);
			}
		}
		

		
		

		glBindFramebuffer(GL_FRAMEBUFFER, 0);



		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPolygonMode(GL_FRONT, GL_FILL);


		// 2. Lighting Pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderLightingPass->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);




		// Also send light relevant uniforms
		for (GLuint i = 0; i < lightPositions.size(); ++i)
		{
			glUniform3fv(glGetUniformLocation(shaderLightingPass->Program, ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &lightPositions[i][0]);
			glUniform3fv(glGetUniformLocation(shaderLightingPass->Program, ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &lightColors[i][0]);
			// Update attenuation parameters and calculate radius
			const GLfloat constant = 1.0f; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
			const GLfloat linear = 0.7f;
			const GLfloat quadratic = 1.8f;
			glUniform1f(glGetUniformLocation(shaderLightingPass->Program, ("lights[" + std::to_string(i) + "].Linear").c_str()), linear);
			glUniform1f(glGetUniformLocation(shaderLightingPass->Program, ("lights[" + std::to_string(i) + "].Quadratic").c_str()), quadratic);
			// Then calculate radius of light volume/sphere
			const GLfloat lightThreshold = 5.0; // 5 / 256
			const GLfloat maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
			GLfloat radius = 10000.f;// (-linear + static_cast<float>(std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0 / lightThreshold) * maxBrightness)))) / (2 * quadratic);
			glUniform1f(glGetUniformLocation(shaderLightingPass->Program, ("lights[" + std::to_string(i) + "].Radius").c_str()), radius);
		}
		glUniform3fv(glGetUniformLocation(shaderLightingPass->Program, "viewPos"), 1, &VK::Camera::GetInstance()->GetPosition()[0]);
		glUniform1i(glGetUniformLocation(shaderLightingPass->Program, "draw_mode"), 4);


		float diffuseConstant(0.7f),
			ambientConstant(0.1f),
			directionalIntensity(1.f);
		StateSystem* p_stateSystem = m_app->getSystem<StateSystem>();
		STATE currState = p_stateSystem->GetCurrentState();

		if (currState == STATE_LOGO || currState == STATE_HOWTOPLAY 
			|| currState == STATE_MAPSELECT)
		{
			diffuseConstant = 1.0f;
		};
		glUniform3f(glGetUniformLocation(shaderLightingPass->Program, "lightDirect"), LightDirction.x, LightDirction.y, LightDirction.z);
		glUniform1f(glGetUniformLocation(shaderLightingPass->Program, "lightIntensity"), directionalIntensity);
		glUniform1f(glGetUniformLocation(shaderLightingPass->Program, "diffuseConstant"), diffuseConstant);

		glUniform3f(glGetUniformLocation(shaderLightingPass->Program, "lightColor"), dirColor.x, dirColor.y, dirColor.z);
		glUniform1f(glGetUniformLocation(shaderLightingPass->Program, "ambientConstant"), ambientConstant);


		int ww = 0, hh = 0;
		glfwGetFramebufferSize(m_app->GetWindow(), &m_windowWidth, &m_windowHeight);
		glfwGetWindowSize(m_app->GetWindow(), &ww, &hh);
		cout << "buffer size : " << m_windowWidth << "  " << m_windowHeight << endl;
		cout << "window size : " << ww << "  " << hh << endl << endl;
		//glViewport(0, 0, m_windowWidth, m_windowHeight);

		RenderQuad();

		// 2.5. Copy content of geometry's depth buffer to default framebuffer's depth buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Write to default framebuffer
												   // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
												   // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
												   // depth buffer in another stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
		glBlitFramebuffer(0, 0, m_windowWidth, m_windowHeight, 0, 0, m_windowWidth, m_windowHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_BLEND);

		//// 3. Render lights on top of scene, by blitting
		shaderLightBox->Use();
		glUniformMatrix4fv(glGetUniformLocation(shaderLightBox->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shaderLightBox->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		for (GLuint i = 0; i < lightPositions.size(); i++)
		{
			model = glm::mat4();
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(50.f));
			glUniformMatrix4fv(glGetUniformLocation(shaderLightBox->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform3fv(glGetUniformLocation(shaderLightBox->Program, "lightColor"), 1, &lightColors[i][0]);
			//RenderCube();
		}
		
		
		//glDepthMask(GL_FALSE);
		//glDisable(GL_DEPTH_TEST);

		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glOrtho(0, windowWidth, 0, windowHeight, CLIP_NEAR, CLIP_FAR);
		////gluOrtho2D(0, windowWidth, 0, windowHeight); //left,right,bottom,top

		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
		////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glColor3f(0.0, 255.0, 0.0);
		//glBegin(GL_QUADS);
		//glVertex2f(50, 50);
		//glVertex2f(50, 100);
		//glVertex2f(100, 100);
		//glVertex2f(100, 50);
		//glEnd();

		//glDepthMask(GL_TRUE);

		//shaderUI->Use();
		//// Render windows (from furthest to nearest)
		//glBindVertexArray(transparentVAO);
		//glBindTexture(GL_TEXTURE_2D, transparentTexture);

		//model = glm::mat4();
		//model = glm::translate(model, glm::vec3(1.f, 1.f, 1.f));
		//glUniformMatrix4fv(glGetUniformLocation(shaderUI->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(0);

		//model = glm::mat4();
		//model = glm::scale(model, glm::vec3(100.f));
		//model = glm::translate(model, glm::vec3(1.f, 1.f, -1.f));
		//glUniformMatrix4fv(glGetUniformLocation(shaderUI->Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(0);

		//glDisable(GL_BLEND);


		////geometryPass();


		////if (glfwGetKey(m_app->GetWindow(), GLFW_KEY_B) == GLFW_PRESS || debugGBuffer) {
		////	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		////	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		////	gbuffer->bindForBlitting();
		////	int windowWidth, windowHeight;
		////	glfwGetWindowSize(m_app->GetWindow(), &windowWidth, &windowHeight);

		////	GLsizei HalfWidth = static_cast<GLsizei>(windowWidth / 2.0f);
		////	GLsizei HalfHeight = static_cast<GLsizei>(windowHeight / 2.0f);

		////	// Lower left
		////	gbuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
		////	glBlitFramebuffer(0, 0, windowWidth, windowHeight,
		////		0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		////	// Upper left
		////	gbuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_DIFFUSE);
		////	glBlitFramebuffer(0, 0, windowWidth, windowHeight,
		////		0, HalfHeight, HalfWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		////	// Upper right
		////	gbuffer->setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
		////	glBlitFramebuffer(0, 0, windowWidth, windowHeight,
		////		HalfWidth, HalfHeight, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);


		////}
		////else {
		////	glEnable(GL_STENCIL_TEST);

		////	for (auto &it : lights)
		////	{
		////		stencilPass(it.second);
		////		pointLightPass(it.second);
		////	}
		////	glDisable(GL_STENCIL_TEST);

		////	directionalLightPass();
		////	finalPass();

		////	/*
		////	// Lower right
		////	gbuffer.setReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_TEXCOORD);
		////	glBlitFramebuffer(0, 0, windowWidth, windowHeight,
		////	HalfWidth, 0, windowWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		////	*/
		////}
	}

	//-----------------------------------------------------------------------------
	// Name: shutDown()
	// Desc: 
	//-----------------------------------------------------------------------------
	void GraphicsRenderer::Shutdown()
	{
		//for (auto & i : models)	
		//{
		//	i.second->~ModelNode();
		//}
		//models.clear();

		//for (auto & i : lights)
		//{
		//	i.second->~LightNode();
		//}
		//lights.clear();

		///*for (auto & i : meshtypes)
		//{
		//	i.second->~Mesh();
		//}*/
		//meshtypes.clear();

		//for (auto & i : shaders)
		//{
		//	i.second->~ShaderProgram();
		//}
		//shaders.clear();

		//	delete m_pCamera;

		//SAFE_DELETE(dirLightPassVertexShader);
		//SAFE_DELETE(dirLightFragShader);
		//SAFE_DELETE(dirLightGeomShader);
		//SAFE_DELETE(stencilVertexShader);
		//SAFE_DELETE(stencilFragShader);

		//SAFE_DELETE(dirLightProgram);

		//SAFE_DELETE(stencilProgram);
	//	SAFE_DELETE(gbuffer);

	}

	void GraphicsRenderer::Update(float dt)
	{
		lightWorks(dt);
		Camera::GetInstance()->UpdateScreenSize();
		if (!m_fullscreen && m_app->m_fullScreen)
		{
			m_fullscreen = true;
			/*gbuffer->~GBuffer();
			delete gbuffer;
			gbuffer = nullptr;*/
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			/*	gbuffer = new GBuffer();
			gbuffer->init(mode->width, mode->height);*/

			//gbuffer->reset(mode->width, mode->height);

			int width, height;
			//glfwGetFramebufferSize(m_app->GetWindow(), &width, &height);
			//glViewport(0, 0, width, height);
			cout << "!mfull" << endl;
			cout << "width " << width << " height : " << height << endl;
		}
		if (m_fullscreen && !m_app->m_fullScreen)
		{
			m_fullscreen = false;

			//gbuffer->reset(m_app->GetPrevWinWidthSize(), m_app->GetPrevWinHeightSize());

			int width, height;
			//glfwGetFramebufferSize(m_app->GetWindow(), &width, &height);
			//glViewport(0, 0, width, height);
			cout << "mfull" << endl;
			cout << "width " << width << " height : " << height << endl;
		}

		Render();
		/*
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window");
		ImGui::Text("");
		ImGui::End();
		*/

	}

	//void GraphicsRenderer::geometryPass()
	//{
	//	//geometryProgram->use();
	//	//shaders["geometryProgram"]->use();
	//	geomPr->Use();
	//	//gbuffer->bindForGeomPass();

	//	// Only the geometry pass updates the depth buffer
	//	glDepthMask(GL_TRUE);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glEnable(GL_DEPTH_TEST);


	//	for (auto &it : models)
	//	{
	//		it.second->geometryPass(this);

	//	}

	//	VK::EntitySystem* pES = m_app->getSystem<VK::EntitySystem>();

	//	STATE currState = m_pStateSystem->GetCurrentState();

	//	if (currState != STATE_PAUSE)
	//	{
	//		VK::EntityListVector e_dynamiclist = pES->GetDynamicEntityList();

	//		for (auto it : e_dynamiclist)
	//		{
	//			//ShaderProgram* program = this->getGeometryProgram();
	//			Shader* program = getGeometryPr();

	//			Camera* camera = VK::Camera::GetInstance();
	//			camera->UpdateCamera();
	//			glm::mat4 mv = camera->GetViewMatrix() * it->FindComponent<TransformComponent>()->getTransformation();
	//			glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(it->FindComponent<TransformComponent>()->getTransformation()));
	//			glm::mat4 mvp = camera->GetProjectionMatrix() * mv;

	//			/*program->setUniformMatrix4fv("model", 1, false, glm::value_ptr(it->FindComponent<TransformComponent>()->getTransformation()));
	//			program->setUniformMatrix4fv("mvp", 1, false, glm::value_ptr(mvp));
	//			program->setUniformMatrix3fv("normalMatrix", 1, false, glm::value_ptr(normalMatrix));*/
	//			glUniformMatrix4fv(glGetUniformLocation(program->Program, "model"), 1, false, glm::value_ptr(it->FindComponent<TransformComponent>()->getTransformation()));
	//			glUniformMatrix4fv(glGetUniformLocation(program->Program, "mvp"), 1, false, glm::value_ptr(mvp));
	//			glUniformMatrix3fv(glGetUniformLocation(program->Program, "normalMatrix"), 1, false, glm::value_ptr(normalMatrix));

	//			if (it->FindComponent<RigidBodyComponent>())
	//			{
	//				//program->setUniform1f("alpha", it->FindComponent<RigidBodyComponent>()->GetHP());
	//				glUniform1f(glGetUniformLocation(program->Program, "alpha"), it->FindComponent<RigidBodyComponent>()->GetHP());
	//			}
	//			else
	//			{
	//				//program->setUniform1f("alpha", 1.f);
	//				glUniform1f(glGetUniformLocation(program->Program, "alpha"), 1.f);
	//			}
	//			//it->FindComponent<MeshComponent>()->getMesh()->Draw(*geomPr);
	//			it->FindComponent<MeshComponent>()->getModel()->Draw(*geomPr);

	//			//glm::vec4 a = glm::vec4(2, 0, 0, 0);
	//			//glm::vec4 b = glm::vec4(0, 2, 0, 0);
	//			//glm::vec4 c = glm::vec4((a.zyx() + b.xyz()).xyz * 0.5f, 1.0f);
	//			// Before being pasted to a function, a swizzle operator needs to be cast into
	//			// a vector either explicitly or with the operator ()
	//			//glm::vec2 d = glm::normalize(glm::vec2(c.yz));
	//			// glm::vec2 d = glm::normalize(c.yz()); // Equivalent code
	//			//a.xyzw = d.xyxy;
	//		}
	//		VK::EntityListVector e_staticlist = pES->GetStaticEntityList();

	//		for (auto it : e_staticlist)
	//		{
	//			if (it->FindComponent<TransformComponent>())
	//			{
	//				//ShaderProgram* program = this->getGeometryProgram();
	//				Shader* program = getGeometryPr();

	//				Camera* camera = VK::Camera::GetInstance();
	//				camera->UpdateCamera();
	//				glm::mat4 mv = camera->GetViewMatrix() * it->FindComponent<TransformComponent>()->getTransformation();
	//				glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(it->FindComponent<TransformComponent>()->getTransformation()));
	//				glm::mat4 mvp = camera->GetProjectionMatrix() * mv;

	//				//program->setUniformMatrix4fv("model", 1, false, glm::value_ptr(it->FindComponent<TransformComponent>()->getTransformation()));
	//				//program->setUniformMatrix4fv("mvp", 1, false, glm::value_ptr(mvp));
	//				//program->setUniformMatrix3fv("normalMatrix", 1, false, glm::value_ptr(normalMatrix));
	//				glUniformMatrix4fv(glGetUniformLocation(program->Program, "model"), 1, false, glm::value_ptr(it->FindComponent<TransformComponent>()->getTransformation()));
	//				glUniformMatrix4fv(glGetUniformLocation(program->Program, "mvp"), 1, false, glm::value_ptr(mvp));
	//				glUniformMatrix3fv(glGetUniformLocation(program->Program, "normalMatrix"), 1, false, glm::value_ptr(normalMatrix));

	//				if (it->FindComponent<RigidBodyComponent>())
	//				{
	//					//program->setUniform1f("alpha", it->FindComponent<RigidBodyComponent>()->GetHP());
	//					glUniform1f(glGetUniformLocation(program->Program, "alpha"), it->FindComponent<RigidBodyComponent>()->GetHP());
	//				}
	//				else
	//				{
	//					//program->setUniform1f("alpha", 1.f);
	//					glUniform1f(glGetUniformLocation(program->Program, "alpha"), 1.f);
	//				}

	//				it->FindComponent<MeshComponent>()->getModel()->Draw(*geomPr);
	//				//it->FindComponent<MeshComponent>()->getMesh()->Draw(*geomPr);
	//			}
	//		}

	//	}
	//	else
	//	{
	//		VK::EntityListVector e_debuglist = pES->GetDebugEntityList();
	//		for (auto it : e_debuglist)
	//		{
	//			//ShaderProgram* program = this->getGeometryProgram();
	//			Shader* program = getGeometryPr();

	//			Camera* camera = VK::Camera::GetInstance();
	//			camera->UpdateCamera();
	//			glm::mat4 mv = camera->GetViewMatrix() * it->FindComponent<TransformComponent>()->getTransformation();
	//			glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(it->FindComponent<TransformComponent>()->getTransformation()));
	//			glm::mat4 mvp = camera->GetProjectionMatrix() * mv;

	//			//program->setUniformMatrix4fv("model", 1, false, glm::value_ptr(it->FindComponent<TransformComponent>()->getTransformation()));
	//			//program->setUniformMatrix4fv("mvp", 1, false, glm::value_ptr(mvp));
	//			//program->setUniformMatrix3fv("normalMatrix", 1, false, glm::value_ptr(normalMatrix));
	//			glUniformMatrix4fv(glGetUniformLocation(program->Program, "model"), 1, false, glm::value_ptr(it->FindComponent<TransformComponent>()->getTransformation()));
	//			glUniformMatrix4fv(glGetUniformLocation(program->Program, "mvp"), 1, false, glm::value_ptr(mvp));
	//			glUniformMatrix3fv(glGetUniformLocation(program->Program, "normalMatrix"), 1, false, glm::value_ptr(normalMatrix));

	//			if (it->FindComponent<RigidBodyComponent>())
	//			{
	//				//program->setUniform1f("alpha", it->FindComponent<RigidBodyComponent>()->GetHP());
	//				glUniform1f(glGetUniformLocation(program->Program, "alpha"), it->FindComponent<RigidBodyComponent>()->GetHP());
	//			}
	//			else
	//			{
	//				//program->setUniform1f("alpha", 1.f);
	//				glUniform1f(glGetUniformLocation(program->Program, "alpha"), 1.f);
	//			}
	//			//it->FindComponent<MeshComponent>()->getMesh()->Draw(*geomPr);
	//			it->FindComponent<MeshComponent>()->getModel()->Draw(*geomPr);
	//		}

	//	}
	//	// Lock down the depth buffer
	//	glDepthMask(GL_FALSE);
	//}
	//void GraphicsRenderer::stencilPass(LightNode* light)
	//{
	//	//gbuffer->bindForStencilPass();
	//	//stencilProgram->use();
	//	stencilPr->Use();

	//	glEnable(GL_DEPTH_TEST);
	//	glDisable(GL_CULL_FACE);
	//	glClear(GL_STENCIL_BUFFER_BIT);

	//	glStencilFunc(GL_ALWAYS, 0, 0);

	//	light->stencilPass(this);
	//}
	//void GraphicsRenderer::pointLightPass(LightNode* light)
	//{
	//	//gbuffer->bindForLightPass();

	//	//pointLightProgram->use();
	//	pointLightPr->Use();

	//	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	//	glDisable(GL_DEPTH_TEST);
	//	glEnable(GL_BLEND);
	//	//glBlendEquation(GL_FUNC_ADD);
	//	//glBlendFunc(GL_ONE, GL_ONE);

	//	glEnable(GL_CULL_FACE);
	//	glCullFace(GL_FRONT);


	//	int windowWidth, windowHeight;
	//	glfwGetWindowSize(m_app->GetWindow(), &windowWidth, &windowHeight);

	//	//float width = static_cast<float>(windowWidth);
	//	//float height = static_cast<float>(windowHeight);



	//	//pointLightProgram->setUniform2f("screenSize", width, height);
	//	//pointLightProgram->setUniform2f("screenSize", static_cast<float>(m_winMaxWidth), static_cast<float>(m_winMaxHeight));
	//	//pointLightProgram->setUniform1f("diffuseConstant", diffuseConstant);
	//	glUniform2f(glGetUniformLocation(pointLightPr->Program, "screenSize"), static_cast<float>(m_winMaxWidth), static_cast<float>(m_winMaxHeight));
	//	glUniform1f(glGetUniformLocation(pointLightPr->Program, "diffuseConstant"), diffuseConstant);

	//	/*pointLightProgram->setUniform1i("positionMap", 0);
	//	pointLightProgram->setUniform1i("colorMap", 1);
	//	pointLightProgram->setUniform1i("normalMap", 2);*/
	//	glUniform1i(glGetUniformLocation(pointLightPr->Program, "positionMap"), 0);
	//	glUniform1i(glGetUniformLocation(pointLightPr->Program, "colorMap"), 1);
	//	glUniform1i(glGetUniformLocation(pointLightPr->Program, "normalMap"), 2);

	//	light->pointLightPass(this);

	//	glCullFace(GL_BACK);
	//	glDisable(GL_BLEND);
	//}
	void GraphicsRenderer::lightWorks(float dt)
	{
		VK::Application* app = m_app;
		VK::Camera* p_cam = VK::Camera::GetInstance();

		glm::vec3 oridir, tardir;
		glm::vec3 diffDir;
		oridir = LightDirction;
		tardir = tardir;

		if (p_cam->GetInstance()->GetCameraState() == VK::Camera_State::Play_pos)
			tardir = glm::vec3(-1, -1, -1);
		if (p_cam->GetInstance()->GetCameraState() == VK::Camera_State::Pause_pos)
			tardir = glm::vec3(-1, -1, -1);
		if (p_cam->GetInstance()->GetCameraState() == VK::Camera_State::Option_pos)
			tardir = glm::vec3(-1, 0, 1);
		if (p_cam->GetInstance()->GetCameraState() == VK::Camera_State::Credit_pos)
			tardir = glm::vec3(1,0,1);
		if (p_cam->GetInstance()->GetCameraState() == VK::Camera_State::Exit_pos)
			tardir = glm::vec3(1, 0, -1);
		if (p_cam->GetInstance()->GetCameraState() == VK::Camera_State::Start_pos)
			tardir = glm::vec3(-1, -1, -1);


		diffDir = oridir - tardir;
		diffDir = diffDir* (dt);

		if (glm::length(diffDir) > 0.005)
			LightDirction = (oridir - diffDir);

		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_OPTION)
			LightDirction = glm::vec3(0, 0, -1);
		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_CREDITS)
			LightDirction = glm::vec3(0, 0, -1);
		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_GAME)
			LightDirction = glm::vec3(-1, -1, -1);
		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_MAPSELECT)
			LightDirction = glm::vec3(-1, -1, -1);
		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_PAUSE)
			LightDirction = glm::vec3(-1, -1, -1);
		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_EDITOR)
			LightDirction = glm::vec3(-1, -1, -1);
		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_WIN)
			LightDirction = glm::vec3(-1, -1, -1);
		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_LOGO)
			LightDirction = glm::vec3(-1, -1, -1);
		if (app->getSystem<VK::StateSystem>()->GetCurrentState() == VK::STATE_HOWTOPLAY)
			LightDirction = glm::vec3(-1, -1, -1);
		if (VK::Camera::GetInstance()->GetCameraState() ==VK::Camera_State::Exit_RUsure)
			LightDirction = glm::vec3(-1, -1, -1);




	}
	//void GraphicsRenderer::directionalLightPass() const
	//{

	//	int windowWidth, windowHeight;
	//	glfwGetWindowSize(m_app->GetWindow(), &windowWidth, &windowHeight);

	//	//dirLightProgram->use();
	//	dirLightPr->Use();
	//	//gbuffer->bindForLightPass();

	//	glDisable(GL_DEPTH_TEST);
	//	glEnable(GL_BLEND);

	//	// Bind shit for the directional light shader
	//	glUniform1i(glGetUniformLocation(dirLightPr->Program, "colorMap"), 1);
	//	glUniform1i(glGetUniformLocation(dirLightPr->Program, "normalMap"), 2);
	//	//dirLightProgram->setUniform1i("colorMap", 1);
	//	//dirLightProgram->setUniform1i("normalMap", 2);


	//	/*dirLightProgram->setUniform3f("lightDir", LightDirction.x, LightDirction.y, LightDirction.z);
	//	dirLightProgram->setUniform1f("lightIntensity", directionalIntensity);
	//	dirLightProgram->setUniform1f("diffuseConstant", diffuseConstant);*/
	//	glUniform3f(glGetUniformLocation(dirLightPr->Program, "lightDir"), LightDirction.x, LightDirction.y, LightDirction.z);
	//	glUniform1f(glGetUniformLocation(dirLightPr->Program, "lightIntensity"), directionalIntensity);
	//	glUniform1f(glGetUniformLocation(dirLightPr->Program, "diffuseConstant"), diffuseConstant);
	//	//dirLightProgram->setUniform2f("screenSize", windowWidth, windowHeight);
	//	//dirLightProgram->setUniform2f("screenSize", static_cast<float>(m_winMaxWidth), static_cast<float>(m_winMaxHeight));
	//	glUniform2f(glGetUniformLocation(dirLightPr->Program, "screenSize"), static_cast<float>(m_winMaxWidth), static_cast<float>(m_winMaxHeight));
	//	//dirLightProgram->setUniform3f("lightColor", 175.f/255.f, 238.f/255.f, 238.f/255.f);
	//	//dirLightProgram->setUniform3f("lightColor", dirColor.x, dirColor.y, dirColor.z);
	//	//dirLightProgram->setUniform1f("ambientConstant", ambientConstant);
	//	glUniform3f(glGetUniformLocation(dirLightPr->Program, "lightColor"), dirColor.x, dirColor.y, dirColor.z);
	//	glUniform1f(glGetUniformLocation(dirLightPr->Program, "ambientConstant"), ambientConstant);

	//	// Geometry shader actually makes the quad
	//	glDrawArrays(GL_POINTS, 0, 1);

	//	glDisable(GL_BLEND);
	//}
	//void GraphicsRenderer::finalPass() const
	//{
	//	//gbuffer->bindForFinalPass();

	//	int windowWidth, windowHeight;
	//	glfwGetWindowSize(m_app->GetWindow(), &windowWidth, &windowHeight);
	//	//glBlitFramebuffer(0, 0, 3280, 2160, 0, 0, 3280, 2160, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	//	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	//}

//	GBuffer* GraphicsRenderer::getGbuffer() const { return gbuffer; }



	/*void GraphicsRenderer::setGeometryPr(Shader* prog) { geomPr = prog; }
	void GraphicsRenderer::setPointLightPr(Shader* prog) { pointLightPr = prog; }
	void GraphicsRenderer::setDirLightPr(Shader* prog) { dirLightPr = prog; }
	void GraphicsRenderer::setStencilPr(Shader* prog) { stencilPr = prog; }

	Shader* GraphicsRenderer::getGeometryPr() const { return geomPr; }
	Shader* GraphicsRenderer::getPointLightPr() const { return pointLightPr; }
	Shader* GraphicsRenderer::getDirLightPr() const { return dirLightPr; }
	Shader* GraphicsRenderer::getStencilPr() const { return stencilPr; }*/


	/*void GraphicsRenderer::setGeometryProgram(ShaderProgram* prog) { geometryProgram = prog; }
	void GraphicsRenderer::setPointLightProgram(ShaderProgram* prog) { pointLightProgram = prog; }
	void GraphicsRenderer::setDirLightProgram(ShaderProgram* prog) { dirLightProgram = prog; }
	void GraphicsRenderer::setStencilProgram(ShaderProgram* prog) { stencilProgram = prog; }

	ShaderProgram* GraphicsRenderer::getGeometryProgram() const { return geometryProgram; }
	ShaderProgram* GraphicsRenderer::getPointLightProgram() const { return pointLightProgram; }
	ShaderProgram* GraphicsRenderer::getDirLightProgram() const { return dirLightProgram; }
	ShaderProgram* GraphicsRenderer::getStencilProgram() const { return stencilProgram; }*/

} //namespace VK

  //gbuffer->init(1980, 1020);
  //Camera camera(glm::vec3(0.0f, 50.0f, 50.0f));
  //addNode(m_pCamera);

  /*meshtypes.emplace("sphere", new Mesh("models/sphere.obj"));
  meshtypes.emplace("cube", new Mesh("models/cube.obj"));
  meshtypes.emplace("character", new Mesh("models/character1.fbx"));
  meshtypes.emplace("LibertStatue", new Mesh("models/LibertStatue.obj"));*/


  //models.emplace("base", new ModelNode());
  //models["base"]->setMesh(meshtypes["cube"]);
  //models["base"]->getTransform().setScale(240.0f, 0.25f, 240.0f);
  //models["base"]->getTransform().setTranslation(0, 0, 0);

  //models.emplace("character1", new ModelNode());
  //models["character1"]->setMesh(meshtypes["character"]);
  //models["character1"]->getTransform().setScale(0.5f);
  //models["character1"]->getTransform().setTranslation(-30.f, 15.f, 30.f);
  //models["character1"]->getTransform().setRotation(C_RIGHT);
  //models["character1"]->direction = S_RIGHT;

  // register physics
  //RegisterPhysicsObject(models, "character1", COMPONENTTYPE::CHARACTER1, 1, 0.3, 0.3, 0,0,100, S_RIGHT);



  //models.emplace("character2", new ModelNode());
  //models["character2"]->setMesh(meshtypes["character"]);
  //models["character2"]->getTransform().setScale(.5f);
  //models["character2"]->getTransform().setTranslation(30.f, 15.f, 30.f);
  //models["character2"]->getTransform().setRotation(C_LEFT);
  //models["character2"]->direction = S_LEFT;

  // register physics
  //RegisterPhysicsObject(models, "character2", COMPONENTTYPE::CHARACTER2, 1, 0.3, 0.3, 1,0,100, S_LEFT);


  //system("cls");
  //aa
  //for (int i = 0; i < PHYSICS->Bodies.size(); ++i)
  //{
  //	std::cout << PHYSICS->Bodies.at(i)->objName << " x : " << PHYSICS->Bodies.at(i)->GetPosition().x << "z : " << PHYSICS->Bodies.at(i)->GetPosition().z << std::endl;
  //}
  ////std::cout << "CHA1 x : " << PHYSICS->Bodies.at(0)->GetPosition().x << "z : " << PHYSICS->Bodies.at(0)->GetPosition().z << std::endl;
  ////std::cout << "CHA2 x : " << PHYSICS->Bodies.at(1)->GetPosition().x << "z : " << PHYSICS->Bodies.at(1)->GetPosition().z << std::endl;
  //std::cout << "HP1 : "<< PHYSICS->Bodies.at(0)->HP << std::endl;
  //std::cout << "HP2 : "<< PHYSICS->Bodies.at(1)->HP << std::endl;

  /*std::cout << "position a : " << "x : " << models["character1"]->getTransform().getTranslation().x << "\ty : " << models["character1"]->getTransform().getTranslation().y << "\tz : " << models["character1"]->getTransform().getTranslation().z << std::endl;
  std::cout << "position a : " << "x : " << models["character2"]->getTransform().getTranslation().x << "\ty : " << models["character2"]->getTransform().getTranslation().y << "\tz : " << models["character2"]->getTransform().getTranslation().z << std::endl;

  std::cout << "position a : " << "x : " << PHYSICS->Bodies.at(0)->Position.x << "\ty : " << PHYSICS->Bodies.at(0)->Position.y << "\tz : " << PHYSICS->Bodies.at(0)->Position.z << std::endl;
  std::cout << "position a : " << "x : " << PHYSICS->Bodies.at(1)->Position.x << "\ty : " << PHYSICS->Bodies.at(1)->Position.y << "\tz : " << PHYSICS->Bodies.at(1)->Position.z << std::endl;*/



  // Camera controls
  /*
  if (p_inputSystem->IsPressed(VK::KEY_W))
  {
  p_cam->ProcessKeyboard(FORWARD, 1);
  }
  if (p_inputSystem->IsPressed(VK::KEY_S))
  {
  p_cam->ProcessKeyboard(BACKWARD, 1);
  }
  if (p_inputSystem->IsPressed(VK::KEY_A))
  {
  p_cam->ProcessKeyboard(LEFT, 1);
  }
  if (p_inputSystem->IsPressed(VK::KEY_D))
  {
  p_cam->ProcessKeyboard(RIGHT, 1);
  }
  */

  //aa
  // first character movement
  //if (PHYSICS->Bodies.at(0)->HP > 0 && PHYSICS->Bodies.at(1)->HP > 0 && !PHYSICS->Bodies.at(0)->isKnockback)
  //{
  //	if (p_inputSystem->IsPressed(VK::KEY_R))
  //	{
  //		models["character1"]->direction = S_UP;
  //		PHYSICS->Bodies.at(0)->Dir = S_UP;
  //		models["character1"]->getTransform().setTranslation(models["character1"]->getTransform().getTranslation() + models["character1"]->direction * 2.f);
  //		models["character1"]->getTransform().setRotation(C_UP);
  //		

  //		updateObject(models, "character1", S_UP * 2.f, 0);

  //	}
  //	if (p_inputSystem->IsPressed(VK::KEY_D))
  //	{
  //		models["character1"]->direction = S_LEFT;
  //		PHYSICS->Bodies.at(0)->Dir = S_LEFT;
  //		models["character1"]->getTransform().setTranslation(models["character1"]->getTransform().getTranslation() + models["character1"]->direction * 2.f);
  //		models["character1"]->getTransform().setRotation(C_LEFT);
  //		
  //		updateObject(models, "character1", S_LEFT * 2.f, 0);
  //	}
  //	if (p_inputSystem->IsPressed(VK::KEY_F))
  //	{
  //		models["character1"]->direction = S_DOWN;
  //		PHYSICS->Bodies.at(0)->Dir = S_DOWN;
  //		models["character1"]->getTransform().setTranslation(models["character1"]->getTransform().getTranslation() + models["character1"]->direction * 2.f);
  //		models["character1"]->getTransform().setRotation(C_DOWN);
  //		
  //		updateObject(models, "character1", S_DOWN * 2.f, 0);
  //	}
  //	if (p_inputSystem->IsPressed(VK::KEY_G))
  //	{
  //		models["character1"]->direction = S_RIGHT;
  //		PHYSICS->Bodies.at(0)->Dir = S_RIGHT;
  //		models["character1"]->getTransform().setTranslation(models["character1"]->getTransform().getTranslation() + models["character1"]->direction * 2.f);
  //		models["character1"]->getTransform().setRotation(C_RIGHT);
  //		
  //		updateObject(models, "character1", S_RIGHT * 2.f, 0);
  //	}
  //	if (p_inputSystem->IsTriggered(VK::KEY_LEFT_SHIFT))
  //		//if (p_inputSystem->IsPressed(VK::KEY_LEFT_SHIFT))
  //	{
  //		addObject("character1", "sphere", "sphere");
  //	}
  //}

  //// second character movement
  //if (PHYSICS->Bodies.at(0)->HP > 0 && PHYSICS->Bodies.at(1)->HP > 0 && !PHYSICS->Bodies.at(1)->isKnockback)
  //{
  //	if (p_inputSystem->IsPressed(VK::KEY_ARROW_UP))
  //	{
  //		models["character2"]->direction = S_UP;
  //		PHYSICS->Bodies.at(1)->Dir = S_UP;
  //		models["character2"]->getTransform().setTranslation(models["character2"]->getTransform().getTranslation() + models["character2"]->direction * 2.f);
  //		models["character2"]->getTransform().setRotation(C_UP);
  //		
  //		updateObject(models, "character2", S_UP * 2.f, 1);
  //	}
  //	if (p_inputSystem->IsPressed(VK::KEY_ARROW_LEFT))
  //	{
  //		models["character2"]->direction = S_LEFT;
  //		PHYSICS->Bodies.at(1)->Dir = S_LEFT;
  //		models["character2"]->getTransform().setTranslation(models["character2"]->getTransform().getTranslation() + models["character2"]->direction * 2.f);
  //		models["character2"]->getTransform().setRotation(C_LEFT);
  //		
  //		updateObject(models, "character2", S_LEFT * 2.f, 1);
  //	}
  //	if (p_inputSystem->IsPressed(VK::KEY_ARROW_DOWN))
  //	{
  //		models["character2"]->direction = S_DOWN;
  //		PHYSICS->Bodies.at(1)->Dir = S_DOWN;
  //		models["character2"]->getTransform().setTranslation(models["character2"]->getTransform().getTranslation() + models["character2"]->direction * 2.f);
  //		models["character2"]->getTransform().setRotation(C_DOWN);
  //		
  //		updateObject(models, "character2", S_DOWN * 2.f, 1);
  //	}
  //	if (p_inputSystem->IsPressed(VK::KEY_ARROW_RIGHT))
  //	{
  //		models["character2"]->direction = S_RIGHT;
  //		PHYSICS->Bodies.at(1)->Dir = S_RIGHT;
  //		models["character2"]->getTransform().setTranslation(models["character2"]->getTransform().getTranslation() + models["character2"]->direction * 2.f);
  //		models["character2"]->getTransform().setRotation(C_RIGHT);
  //		
  //		updateObject(models, "character2", S_RIGHT * 2.f, 1);
  //	}

  //	if (p_inputSystem->IsTriggered(VK::KEY_RIGHT_SHIFT))
  //		//if (p_inputSystem->IsPressed(VK::KEY_RIGHT_SHIFT))
  //	{
  //		addObject("character2", "sphere", "sphere");
  //	}
  //}

  //models["character1"]->getTransform().setTranslation(PHYSICS->Bodies.at(0)->GetPosition());
  //models["character2"]->getTransform().setTranslation(PHYSICS->Bodies.at(1)->GetPosition());
  //ShapeAAB *tt;
  ////int i = 2;

  //

  //for (int i = 2; i < PHYSICS->Bodies.size(); ++i)
  //{
  //	if (PHYSICS->Bodies.at(i)->isActive)
  //	{
  //		models[PHYSICS->Bodies.at(i)->objName]->getTransform().setTranslation(PHYSICS->Bodies.at(i)->GetPosition() + PHYSICS->Bodies.at(i)->Velocity);
  //		PHYSICS->Bodies.at(i)->SetPosition(PHYSICS->Bodies.at(i)->GetPosition() + PHYSICS->Bodies.at(i)->Velocity);
  //	}
  //	else
  //		models[PHYSICS->Bodies.at(i)->objName]->getTransform().setTranslation(PHYSICS->Bodies.at(i)->GetPosition());

  //	if (models[PHYSICS->Bodies.at(i)->objName]->getTransform().getScale().x < 10.f)
  //		models[PHYSICS->Bodies.at(i)->objName]->getTransform().setScale(models[PHYSICS->Bodies.at(i)->objName]->getTransform().getScale().x + 3.f * dt);

  //	if (PHYSICS->Bodies.at(i)->GetPosition().x > 200.f ||
  //		PHYSICS->Bodies.at(i)->GetPosition().x < -200.f ||
  //		PHYSICS->Bodies.at(i)->GetPosition().z > 200.f ||
  //		PHYSICS->Bodies.at(i)->GetPosition().z < -200.f)
  //	{
  //		models.erase(PHYSICS->Bodies.at(i)->objName);
  //		PHYSICS->Bodies.erase(PHYSICS->Bodies.begin()+i);
  //		--i;
  //	}
  //}


  //if (PHYSICS->Bodies.at(0)->HP <= 0)
  //{
  //	models["character1"]->getTransform().setTranslation(1000.f, 1000.f, 1000.f);
  //	models["character2"]->getTransform().setTranslation(0.f, 15.f, -30.f);
  //	models["character2"]->getTransform().setRotation(C_DOWN);
  //	dirColor = glm::vec3(255.f / 255.f, 102.f / 255.f, 255.f / 255.f);

  //	for (std::unordered_map<std::string, ModelNode*>::iterator it = models.begin(); it != models.end();)
  //	{
  //		if (it->second->types == "snow")
  //		{
  //			it = models.erase(it);
  //		}
  //		else
  //		{
  //			++it;
  //		}
  //	}
  //	for (int j = 0; j < PHYSICS->Bodies.size(); ++j)
  //	{
  //		if (PHYSICS->Bodies.at(j)->types == COMPONENTTYPE::SNOW1 || PHYSICS->Bodies.at(j)->types == COMPONENTTYPE::SNOW2)
  //		{
  //			//PHYSICS->Bodies.erase(PHYSICS->Bodies.at(j));
  //			--j;
  //		}
  //	}
  //}
  //if (PHYSICS->Bodies.at(1)->HP <= 0)
  //{
  //	models["character2"]->getTransform().setTranslation(1000.f, 1000.f, 1000.f);
  //	models["character1"]->getTransform().setTranslation(0.f, 15.f, -30.f);
  //	models["character1"]->getTransform().setRotation(C_DOWN);
  //	dirColor = glm::vec3(0.f / 255.f, 255.f / 255.f, 0.f / 255.f);

  //	//models.rehash(models.size());


  //	for (std::unordered_map<std::string, ModelNode*>::iterator it = models.begin(); it != models.end();)
  //	{
  //		if (it->second->types == "snow")
  //		{
  //			it = models.erase(it);
  //		}
  //		else
  //		{
  //			++it;
  //		}
  //	}
  //	for (int j = 0; j < PHYSICS->Bodies.size(); ++j)
  //	{
  //		if (PHYSICS->Bodies.at(j)->types == COMPONENTTYPE::SNOW1 || PHYSICS->Bodies.at(j)->types == COMPONENTTYPE::SNOW2)
  //		{
  //			//PHYSICS->Bodies.erase(PHYSICS->Bodies.at(j));
  //			--j;
  //		}
  //	}
  //}

  //character.at(0)->getTransform().setScale(character.at(0)->getTransform().getScale() * 1.01f);

  // Start the frame
  //ImGui::NewFrame();