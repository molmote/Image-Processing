/*****************************************************************************/
/*!
\file   ImguiManager.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for ImguiManager features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "System.h"
#include <vector>
#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include "Application.h"

namespace VK
{
	class Camera;
	class BaseShapeRenderer;
	class LightRenderer;
	class Message;
	class ObjectProporties;
	class EntitySystem;
	class Entity;
	class GraphicsRenderer;
	class AudioSystem;

	class ImguiMessage : public Message
	{
		ImguiMessage() : Message(MsgId::IMGUI_INPUT) {};

	};

	class ImguiManager : public System
	{
	public:
		ImguiManager(void);
		~ImguiManager(void);

		virtual void Init();
		virtual void Update(float dt);
		virtual void Shutdown();
		//virtual void ProcessMessage(Message* message);

		void NewFrame();

		void Render();
		void ProcessMessage(Message* message) override;
		void SetEditorSwitch(bool button) { m_imguiSwitch = button; }
		bool GetEditorSwitch(void) { return m_imguiSwitch; }


		void MB();
		//bool ImGui_ImplGlfw_CreateDeviceObjects();

		static ImguiManager* GetInstance();

		__inline Camera* GetCamera() { return m_pCamera; }

	private:
		std::vector<BaseShapeRenderer*> m_vecShapes;
		std::vector<ObjectProporties*> m_vecObjToRender;
		Camera* m_pCamera;
		GLFWwindow* m_window;


		//INTEGRATED
		int m_nRenderMode;



		static ImguiManager* m_pInstance;

		//vince

		void Editor(void);
		void EntityListWindow(void);
		void CmpListWindow(Entity* entity);
		void SoundEditorWindow(void);

		bool m_imguiSwitch;
		bool m_cmpList_window;
		bool m_entityList_window;
		bool m_soundEditor_window;
		int m_entityType;
		Application* app = Application::GetAppInstance();
		EntitySystem* m_p_entitySystem;
		AudioSystem* m_p_audioSystem;
		GraphicsRenderer* m_p_graphicRenderer;
		std::vector<bool> cmpWindowBool;
		Entity* entity = nullptr;
		std::string str;
		

	};
} // namespace VK