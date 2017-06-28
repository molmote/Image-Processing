/*****************************************************************************/
/*!
\file   Camera.cpp
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
#include "Camera.h"
#include "Message.h"
#include "Application.h"
#include "StateSystem.h"
namespace VK
{
	Camera* Camera::m_Instance = nullptr;

	// Constructor with vectors
	Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
		: Front(glm::vec3(0.0f, -0.5f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		GLFWmonitor * monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		//glfwSetWindowMonitor(m_window, monitor, 0,0,mode->width, mode->height, mode->refreshRate);

		width = mode->width;
		height = mode->height;

		this->Position = position;
		this->WorldUp = up;
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->UpdateCamera();
		camerastate = 0;
	}
	// Constructor with scalar values
	Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), width(1366), height(768)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->UpdateCamera();
	}
	void Camera::SetCameraState(int state)
	{
		camerastate = state;
	}
	int Camera::GetCameraState() const
	{
		return camerastate;
	}
	Camera::~Camera()
	{
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	// Returns the projection matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 Camera::GetProjectionMatrix() const
	{
		return glm::perspective(Zoom, static_cast<GLfloat>(width) / static_cast<GLfloat>(height), CLIP_NEAR, CLIP_FAR);
	}

	glm::vec3 Camera::GetPosition() const
	{
		return Position;
	}

	void Camera::SetPosition(glm::vec3 pos)
	{
		Position = pos;
	}

	glm::vec3 Camera::GetFront() const
	{
		return Front;
	}

	void Camera::SetFront(glm::vec3 front)
	{
		Front = front;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
	}
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->UpdateCamera();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void Camera::ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}

	void Camera::UpdateCamera()
	{
		VK::Application* app = VK::Application::GetAppInstance();
		VK::StateSystem* m_pSS = app->getSystem<VK::StateSystem>();
		// Calculate the new Front vector

		STATE curr = m_pSS->GetCurrentState();
		if (curr == STATE_EDITOR)
		{
			glm::vec3 front;
			front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
			front.y = sin(glm::radians(this->Pitch));
			front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
			this->Front = glm::normalize(front);
		}
		
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
	}

	void Camera::UpdateScreenSize()
	{
		GLFWwindow * window = Application::GetAppInstance()->GetWindow();

		//int width, height;
		glfwGetWindowSize(window, &width, &height);
	}

	void Camera::ProcessMessage(Message* message)
	{
	}
	Camera* Camera::GetInstance()
	{
		if (!m_Instance)
			m_Instance = new Camera(glm::vec3(0.f, 300.f, 600.f));
		return m_Instance;

	}
} //namespace VK