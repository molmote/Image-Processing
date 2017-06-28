/*****************************************************************************/
/*!
\file   Camera.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Camera features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"

namespace VK
{
	enum Camera_State {
		Start_pos,
		Play_pos,
		Option_pos,
		Credit_pos,
		Exit_pos,
		Pause_pos,
		Exit_RUsure
	};
	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Default camera values
	const static GLfloat YAW = -90.0f;
	const static GLfloat PITCH = -40.0f;
	const static GLfloat SPEED = 3.0f;
	const static GLfloat SENSITIVTY = 0.10f;
	const static GLfloat ZOOM = 45.0f;


	class Message;

	class Camera
	{
	public:
		// Constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);

		// Constructor with scalar values
		Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

		~Camera();

		// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix() const;

		// Returns the projection matrix calculated using Eular Angles and the LookAt Matrix
		glm::mat4 GetProjectionMatrix() const;
		glm::vec3 GetPosition() const;
		void SetPosition(glm::vec3 pos);

		glm::vec3 GetFront() const;
		void SetFront(glm::vec3 front);

		// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
		void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

		// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
		void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

		// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
		void ProcessMouseScroll(GLfloat yoffset);

		void UpdateCamera();

		void UpdateScreenSize();

		void ProcessMessage(Message* message);

		static Camera* GetInstance();
		void SetCameraState(int state);
		int GetCameraState() const;
	private:
		// Camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		// Eular Angles
		GLfloat Yaw;
		GLfloat Pitch;
		// Camera options
		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Zoom;

		static Camera* m_Instance;

	private:
		int width;
		int height;
		int camerastate;
	};

} //namespace VK