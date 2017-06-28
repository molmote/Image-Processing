/*****************************************************************************/
/*!
\file   GraphicsRenderer.h
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
#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <unordered_map>

#include "System.h"
#include "Mesh.h"
#include "GBuffer.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "model.h"

class IModel;

namespace VK
{
	class Application;


	//class CameraNode;
	
	class Message;
	//class Device;
	class ShaderProgram;
	class StateSystem;


	class GraphicsRenderer : public System
	{
	public:
		GraphicsRenderer();
		~GraphicsRenderer();

		void Init() override;
		void Update(float dt) override;
		void Shutdown() override;
		void ProcessMessage(Message* message) override;
		
		void	setSize(int w, int h);
		void	release();

		void Render();
		//void ProcessMessage(Message* message);





		static GraphicsRenderer* GetInstance();

		//		__inline Camera* GetCamera() { return m_pCamera; }

		
		glm::vec3 dirColor;

		bool m_fullscreen;

		int m_windowWidth;
		int m_windowHeight;


		//INTEGRATED
		int m_nRenderMode;		

		GLuint gBuffer;
		GLuint gPosition, gNormal, gAlbedoSpec;

		GLuint rboDepth;

		Model *cyborg;
		Shader *shaderGeometryPass;
		Shader *shaderLightingPass;
		Shader *shaderLightBox;

		std::vector<glm::vec3> objectPositions;
		std::vector<glm::vec3> lightPositions;
		std::vector<glm::vec3> lightColors;

		Shader *shaderUI;
		GLuint planeVAO, planeVBO;
		GLuint transparentVAO, transparentVBO;
		

	private:

		Application* m_app;

		StateSystem* m_pStateSystem;

		
		float m_lightRotation;
		//void geometryPass();

		glm::vec3 LightDirction;

		unsigned objnumber;

		static GraphicsRenderer* m_pInstance;
		void lightWorks(float dt);
	};
} // namespace VK