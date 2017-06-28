/*****************************************************************************/
/*!
\file   Application.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for Credit state of Application
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "Application.h"
#include "System.h"
#include "AudioSystem.h"
#include "EntitySystem.h"
#include "InputSystem.h"
#include "StateSystem.h"
#include "ImguiManager.h"
#include "GraphicsRenderer.h"
#include "Game.h"
#include "Physics.h"
//demo
#include "Camera.h"
#include "imgui.h"
#include "GBuffer.h"
#include "Factory.h"
#include "Option.h"
namespace VK
{
	Application* Application::m_appInstance = nullptr;
	static double g_zoom = 0;
	static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_key_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos);

	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	//static void window_focus_callback(GLFWwindow* window, int focused);

	static int swap_tear;
	static int swap_interval;
	static double frame_rate;
	static void update_window_title(GLFWwindow* window)
	{
		char title[256];

		snprintf(title, sizeof(title), "Alaska Roll (interval %i%s, %0.1f Hz)",
			swap_interval,
			(swap_tear && swap_interval < 0) ? " (swap tear)" : "",
			frame_rate);

		glfwSetWindowTitle(window, title);
	}

	static void set_swap_interval(GLFWwindow* window, int interval)
	{
		swap_interval = interval;
		glfwSwapInterval(swap_interval);
		update_window_title(window);
	}

	Application::Application(): m_mode(nullptr), monitor(nullptr), m_stateManager(nullptr)
	{
		m_Active = true;
		m_currentTime = 0.f;

		m_displayMode = true;
		m_fullScreen = false;

		m_fullScreenWidth = 1920;
		m_fullScreenHeight = 1080;
		//abc = false;

		m_prevResolution = ResolutionType::FirstFLAG;
		m_currentResolution = ResolutionType::FirstFLAG;

		
			
		//m_resolution.push_back(make_pair<int, int>(1920, 1080));
		/*m_resolution =
		{
			std::make_tuple(1920, 1080),
			std::make_tuple(1600, 900),
			std::make_tuple(1280, 720),
			std::make_tuple(800, 600)
		};*/
		m_resolution =
		{
			std::make_pair<int,int>(1920, 1080),
			std::make_pair<int,int>(1600, 900),
			std::make_pair<int,int>(1280, 720),
			std::make_pair<int,int>(800, 600)
		};

		m_window = nullptr;
		m_mainwindow = nullptr;
	}
	Application::Application(int resolution, bool fullscreen) : m_mode(nullptr), monitor(nullptr), m_stateManager(nullptr), m_fullScreen(fullscreen)
	{
		m_Active = true;
		m_currentTime = 0.f;

		m_displayMode = false;

		switch(static_cast<ResoultionSize>(resolution))
		{
		case R1920_1080:
			m_fullScreenWidth = 1920;
			m_fullScreenHeight = 1080;
			break;
		case R1680_1050:
			m_fullScreenWidth = 1680;
			m_fullScreenHeight = 1050; 
			break;
		case R1280_800:
			m_fullScreenWidth = 1280;
			m_fullScreenHeight = 800; 
			break;
		case R1280_720:
			m_fullScreenWidth = 1280;
			m_fullScreenHeight = 720; 
			break;
		case R1366_768:
			m_fullScreenWidth = 1366;
			m_fullScreenHeight = 768; 
			break;
		case R1024_768:
			m_fullScreenWidth = 1024;
			m_fullScreenHeight = 768; 
			break;
		case R800_600:
			m_fullScreenWidth = 800;
			m_fullScreenHeight = 600; 
			break;
		default: //-1
			m_fullScreenWidth = 1920;
			m_fullScreenHeight = 1080;
		}
		/*m_fullScreenWidth = 1920;
		m_fullScreenHeight = 1080;*/
		//abc = false;

		m_prevResolution = ResolutionType::FirstFLAG;
		m_currentResolution = ResolutionType::FirstFLAG;



		//m_resolution.push_back(make_pair<int, int>(1920, 1080));
		/*m_resolution =
		{
		std::make_tuple(1920, 1080),
		std::make_tuple(1600, 900),
		std::make_tuple(1280, 720),
		std::make_tuple(800, 600)
		};*/
		m_resolution =
		{
			std::make_pair<int,int>(1920, 1080),
			std::make_pair<int,int>(1600, 900),
			std::make_pair<int,int>(1280, 720),
			std::make_pair<int,int>(800, 600)
		};

		m_window = nullptr;
		m_mainwindow = nullptr;
	}
	Application::~Application()
	{
		glfwTerminate();

		//for (SystemVector::iterator it = m_systemList.begin(); it != m_systemList.end(); it++)
		//{
		//	delete it->pSystem;
		//}
		for (auto & it : m_systemList)
		{
			delete it.pSystem;
		}
	}
	void Application::Initialize()
	{
		START();
		
		//start = std::chrono::system_clock::now();



		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwSetWindowFocusCallback(m_window, window_focus_callback);

		if (!glfwInit()) {
			exit(EXIT_FAILURE);
		}

		monitor = glfwGetPrimaryMonitor();
		m_mode = (GLFWvidmode*)glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
		//		m_mainwindow = glfwCreateWindow(1, 1, "0", NULL, NULL);

		Init();

		//todo
		//Recreate();

		//if (m_fullScreen)
		//{
		//	if (m_mainwindow == nullptr)
		//		m_mainwindow = glfwCreateWindow(std::get<0>(m_resolution[static_cast<int>(m_currentResolution)]), std::get<1>(m_resolution[static_cast<int>(m_currentResolution)]), "GAM moo", glfwGetPrimaryMonitor(), NULL);
		//}
		//else
		//{
		//	if (m_mainwindow == nullptr)
		//		m_mainwindow = glfwCreateWindow(std::get<0>(m_resolution[static_cast<int>(m_currentResolution)]), std::get<1>(m_resolution[static_cast<int>(m_currentResolution)]), "GAM moo", NULL, NULL);
		//}
		//glfwMakeContextCurrent(m_mainwindow);

		//if (!m_mainwindow)
		//{
		//	glfwTerminate();
		//	return;
		//}

		////glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

	

		//int width, height;
		//glfwGetWindowSize(m_mainwindow, &width, &height);
		//glfwSetWindowAspectRatio(m_mainwindow, width, height);

		////glfwSetWindowSizeCallback(m_window, window_size_callback);
		//glfwSetFramebufferSizeCallback(m_mainwindow, framebuffer_size_callback);

		//// set window at center
		//glfwSetWindowPos(m_mainwindow, (m_mode->width - width) / 2 , (m_mode->height - height) / 2);

		//glfwSetWindowSizeLimits(m_mainwindow, m_mode->width / 3, m_mode->height / 3, m_mode->width, m_mode->height);

		////keyboard callback
		//glfwSetKeyCallback(m_mainwindow, keyboard_callback);
		////mouse button callback
		//glfwSetMouseButtonCallback(m_mainwindow, mouse_key_callback);
		////mouse scroll callback
		//glfwSetScrollCallback(m_mainwindow, mouse_scroll_callback);
		////mouse position callback
		//glfwSetCursorPosCallback(m_mainwindow, mouse_pos_callback);
		//glfwMakeContextCurrent(m_mainwindow);
		//glewInit();

		//// Options
		//glfwSetInputMode(m_mainwindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		//// Accept fragment if it closer to the camera than the former one
		//glDepthFunc(GL_LESS);
		// Cull triangles with normals not towards camera

		//glEnable(GL_CULL_FACE);
		m_currentTime = static_cast<float>(glfwGetTime());


		for each(auto& it in m_systemList)
		{
			it.pSystem->Init();
		}
		last_time = glfwGetTime();
		frame_rate = 0.0;
		swap_tear = (glfwExtensionSupported("WGL_EXT_swap_control_tear") ||
			glfwExtensionSupported("GLX_EXT_swap_control_tear"));
		LOG("initialize all the system");
		/*int count;
		const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);*/
		int count;
		const GLFWvidmode* modes = glfwGetVideoModes(monitor, &count);
		float currentRatio = float(modes[count - 1].width) / float(modes[count - 1].height);
		for(int i = 0; i < count; ++i)
		{
			float ratio = float(modes[i].width) / float(modes[i].height);
			if (currentRatio == ratio)
			{
				//m_resolution.fill(std::make_tuple(modes[i].width, modes[i].height));
				//cout << modes[i].width << " * " << modes[i].height << endl;
			}
		}

		//todo it's just stand for checking the list
		/*cout << "size" << m_resolution.size() << endl;
		for(int i = 0; i < m_resolution.size(); ++i)
		{
			cout << "sdfsdf";
			cout << std::get<0>(m_resolution[i]) << " * " << std::get<1>(m_resolution[i]) << endl;
		}*/
			
		/*int maxWidth = 0;
		int maxHeight = 0;
		for (int i = 0; i < count; i++)
		{
		if (modes[i].width > maxWidth)
		maxWidth = modes[i].width;
		if (modes[i].height > maxHeight)
		maxHeight = modes[i].height;
		}*/
 	}
	void Application::Run(void)
	{

		float startTime = static_cast<float>(glfwGetTime());
		float dt = 0.f;
		while (!glfwWindowShouldClose(m_mainwindow) && m_Active)
		{
			FocusWindow();

			m_currentTime = static_cast<float>(glfwGetTime());

			dt = m_currentTime - startTime;

			startTime = m_currentTime;

			for each(auto& it in m_systemList)
			{
				it.pSystem->Update(dt);
			}

			glfwSwapBuffers(m_mainwindow);
			glfwPollEvents();
			++frame_count;

#ifdef _DEBUG
			current_time = glfwGetTime();
			if (current_time - last_time > 1.0)
			{
				frame_rate = frame_count / (current_time - last_time);
				frame_count = 0;
				last_time = current_time;
				update_window_title(m_window);
			}
#endif

			if (m_prevResolution != m_currentResolution)
			{
				m_prevResolution = m_currentResolution;
				glfwSetWindowSize(m_mainwindow, std::get<0>(m_resolution[static_cast<int>(m_currentResolution)]), std::get<1>(m_resolution[static_cast<int>(m_currentResolution)]));
			}
		}

	}
	void Application::Shutdown() const
	{
		for each(auto& it in m_systemList)
		{
			it.pSystem->Shutdown();
		}
	}
	Application* Application::GetAppInstance()
	{
		if (!m_appInstance)
			m_appInstance = new Application();
		return m_appInstance;
	}
	Application* Application::GetAppInstance(int resolution, bool fullscreen)
	{
		if (!m_appInstance)
			m_appInstance = new Application(resolution, fullscreen);
		return m_appInstance;
	}

	void Application::BroadcastMessage(Message* message) const
	{
		for each(auto& it in m_systemList)
		{
			it.pSystem->ProcessMessage(message);
		}
	}


	void Application::FocusWindow()
	{
		AudioSystem* p_audioSystem = getSystem<AudioSystem>();
		if (glfwGetWindowAttrib(m_mainwindow, GLFW_FOCUSED))//focused
		{
			p_audioSystem->Pause_Sound_All(false);

		}
		else//unfocused
		{
			StateSystem* p_stateSystem = getSystem<StateSystem>();
			STATE currState = p_stateSystem->GetCurrentState();

			if (currState== STATE_GAME)
			{
				p_stateSystem->PauseAndSetNext(STATE_PAUSE);
			}
			p_audioSystem->Pause_Sound_All(true);
			glfwIconifyWindow(m_mainwindow);
		}
	}

	void Application::Init()
	{
		if (m_fullScreen)
		{
			glfwWindowHint(GLFW_RED_BITS, m_mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, m_mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, m_mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, m_mode->refreshRate);

			m_window = glfwCreateWindow(m_mode->width, m_mode->height, "Alaska Roll", glfwGetPrimaryMonitor(), m_mainwindow);
			glfwDestroyWindow(m_mainwindow);
			m_mainwindow = m_window;
			glfwMakeContextCurrent(m_mainwindow);
			glfwShowWindow(m_mainwindow);
		}
		else
		{
			m_window = glfwCreateWindow(std::get<0>(m_resolution[static_cast<int>(m_currentResolution)]), std::get<1>(m_resolution[static_cast<int>(m_currentResolution)]), "Alaska Roll", nullptr, m_mainwindow);
			glfwDestroyWindow(m_mainwindow);
			m_mainwindow = m_window;
			glfwMakeContextCurrent(m_mainwindow);
			glfwShowWindow(m_mainwindow);
		}
		if (!m_mainwindow)
		{
			glfwTerminate();
			return;
		}

		//glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);



		int width, height;
		glfwGetWindowSize(m_mainwindow, &width, &height);
		glfwSetWindowAspectRatio(m_mainwindow, width, height);

		//glfwSetWindowSizeCallback(m_window, window_size_callback);
		glfwSetFramebufferSizeCallback(m_mainwindow, framebuffer_size_callback);

		// set window at center
		glfwSetWindowPos(m_mainwindow, (m_mode->width - width) / 2, (m_mode->height - height) / 2);

		glfwSetWindowSizeLimits(m_mainwindow, m_mode->width / 3, m_mode->height / 3, m_mode->width, m_mode->height);

		//keyboard callback
		glfwSetKeyCallback(m_mainwindow, keyboard_callback);
		//mouse button callback
		glfwSetMouseButtonCallback(m_mainwindow, mouse_key_callback);
		//mouse scroll callback
		glfwSetScrollCallback(m_mainwindow, mouse_scroll_callback);
		//mouse position callback
		glfwSetCursorPosCallback(m_mainwindow, mouse_pos_callback);
		//glfwMakeContextCurrent(m_mainwindow);
		glewInit();

		// Options
		glfwSetInputMode(m_mainwindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		// Cull triangles with normals not towards camera
		//getSystem<GraphicsRenderer>()->Init();
		
		glEnable(GL_CULL_FACE);
	}
	void Application::Recreate()
	{
		static bool change = false;
		if (m_fullScreen)
		{
			glfwWindowHint(GLFW_RED_BITS, m_mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, m_mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, m_mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, m_mode->refreshRate);

			m_window = glfwCreateWindow(m_mode->width, m_mode->height, "Alaska Roll", glfwGetPrimaryMonitor(), m_mainwindow);
			glfwDestroyWindow(m_mainwindow);
			m_mainwindow = m_window;
			glfwMakeContextCurrent(m_mainwindow);
			glfwShowWindow(m_mainwindow);
			m_systemList.at(2).pSystem->Shutdown();
			change = true;
		}
		else
		{
			m_window = glfwCreateWindow(std::get<0>(m_resolution[static_cast<int>(m_currentResolution)]), std::get<1>(m_resolution[static_cast<int>(m_currentResolution)]), "Alaska Roll", nullptr, m_mainwindow);
			glfwDestroyWindow(m_mainwindow);
			
			m_mainwindow = m_window;
			glfwMakeContextCurrent(m_mainwindow);
			glfwShowWindow(m_mainwindow);
			/*ResourceManager::GetInstance()->~ResourceManager();*/
			//getSystem<GraphicsRenderer>()->~GraphicsRenderer();
			//deleteGraphic();
			m_systemList.at(2).pSystem->Shutdown();
			change = true;

		}
		m_fullScreen = !m_fullScreen;
		if (!m_mainwindow)
		{
			glfwTerminate();
			return;
		}

		//glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);



		int width, height;
		glfwGetWindowSize(m_mainwindow, &width, &height);
		glfwSetWindowAspectRatio(m_mainwindow, width, height);

		//glfwSetWindowSizeCallback(m_window, window_size_callback);
		glfwSetFramebufferSizeCallback(m_mainwindow, framebuffer_size_callback);

		// set window at center
		glfwSetWindowPos(m_mainwindow, (m_mode->width - width) / 2, (m_mode->height - height) / 2);

		glfwSetWindowSizeLimits(m_mainwindow, m_mode->width / 3, m_mode->height / 3, m_mode->width, m_mode->height);

		//keyboard callback
		glfwSetKeyCallback(m_mainwindow, keyboard_callback);
		//mouse button callback
		glfwSetMouseButtonCallback(m_mainwindow, mouse_key_callback);
		//mouse scroll callback
		glfwSetScrollCallback(m_mainwindow, mouse_scroll_callback);
		//mouse position callback
		glfwSetCursorPosCallback(m_mainwindow, mouse_pos_callback);
		//glfwMakeContextCurrent(m_mainwindow);
		glewInit();

		// Options
		glfwSetInputMode(m_mainwindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		// Cull triangles with normals not towards camera
		//getSystem<GraphicsRenderer>()->Init();
		if (change)
		{
			std::vector<std::string> tempList = ResourceManager::GetInstance()->GetModelList();
			ResourceManager::GetInstance()->~ResourceManager();
			getSystem<EntitySystem>()->ClearEntity();
			//Factory::CreateLevel("Option.Json");
			//dynamic_cast<Option*>(getSystem<StateSystem>()->m_stateList[STATE_OPTION])->m_OptionState = OptionState::Resolution;
			getSystem<StateSystem>()->m_stateList[STATE_OPTION]->Enter();
			delete m_systemList.at(2).pSystem;
			m_systemList.at(2).pSystem = new GraphicsRenderer();
			m_systemList.at(2).pSystem->Init();
			//GraphicsRenderer::GetInstance()->Init();
			//getSystem<GraphicsRenderer>()->Init();
			
			//for (size_t i = 0; i < tempList.size(); ++i)
			//{
			//	ResourceManager::GetInstance()->setModel(tempList[i]);
			//}
			change = false;
		}
		glEnable(GL_CULL_FACE);
	}

	ResolutionType Application::GetPrevResolution() const { return m_prevResolution; }

	void Application::SetCurrentResolution(ResolutionType state) { m_currentResolution = state; }

	void Application::ToggleFullscreen() { m_fullScreen = !m_fullScreen; }


	//note that these functions used for toggle screen so that it return currentResolution.
	int Application::GetPrevWinWidthSize() { return std::get<0>(m_resolution[static_cast<int>(m_currentResolution)]); }
	int Application::GetPrevWinHeightSize() { return  std::get<1>(m_resolution[static_cast<int>(m_currentResolution)]); }



	static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Application* app = Application::GetAppInstance();
		InputSystem* p_inputSystem = app->getSystem<InputSystem>();
		ImguiManager* p_imguiSystem = app->getSystem<ImguiManager>();

		bool editorCheck = p_imguiSystem->GetEditorSwitch();
		if (editorCheck)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (action == GLFW_PRESS)
				io.KeysDown[key] = true;
			if (action == GLFW_RELEASE)
				io.KeysDown[key] = false;

			(void)mods; // Modifiers are not reliable across systems
			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
			io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	
		}
		else
		{
			if (action == GLFW_PRESS)
			{
				p_inputSystem->InputKeyDown(p_inputSystem->InputTranslateKey(key));
			}
			else if (action == GLFW_RELEASE)
			{
				p_inputSystem->InputKeyUp(p_inputSystem->InputTranslateKey(key));
			}
			else
				return;
		}
		
	}

	static void mouse_key_callback(GLFWwindow* window, int button, int action, int mods)
	{
		Application* app = Application::GetAppInstance();
		InputSystem* p_inputSystem = app->getSystem<InputSystem>();
		if (action == GLFW_PRESS)
		{
			p_inputSystem->InputKeyDown(p_inputSystem->InputTranslateKey(button));
		}
		else if (action == GLFW_RELEASE)
		{
			p_inputSystem->InputKeyUp(p_inputSystem->InputTranslateKey(button));
		}
		else
			return;
	}

	static void mouse_scroll_callback(GLFWwindow* window,double xoffset,double yoffset)
	{
		g_zoom += static_cast<float>(yoffset); // Use fractional mouse wheel, 1.0 unit 5 lines.
		VK::Camera::GetInstance()->ProcessMouseScroll(static_cast<float>(yoffset));

	}
	static void mouse_pos_callback(GLFWwindow* window,double xpos,double ypos)
	{
		Application* app = Application::GetAppInstance();
		InputSystem* p_inputSystem = app->getSystem<InputSystem>();
		p_inputSystem->SetMouseX(xpos);
		p_inputSystem->SetMouseY(ypos);
	}

	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		//int width, height;
		//glfwGetWindowSize(window, &width, &height);
		glfwSetWindowSize(window, width, height);
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}
