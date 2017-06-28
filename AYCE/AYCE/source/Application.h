/*****************************************************************************/
/*!
\file   Application.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Credits features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

//#include <GLFW/glfw3.h>
#include <vector>
#include <typeinfo>

#include "precompiled.h"
namespace VK
{
	enum class ResolutionType {
		FirstFLAG,
		//by1366768 = 0,
		by19201080 = 0,
		by1600900,
		by1280720,
		by800600,
		LASTFLAG
	};

	enum ResoultionSize
	{
		ResoultionSize_First = 0,
		R1920_1080 = ResoultionSize_First,
		R1680_1050,
		R1280_800,
		R1280_720,
		R1366_768,
		R1024_768,
		R800_600,
		ResoultionSize_Size,
	};

	class Message;
	class System;
	class StateManager;
	struct SystemData
	{
		SystemData(): pSystem(nullptr)
		{
		} ;
		std::string name;
		System* pSystem;
	};
	typedef std::vector<SystemData> SystemVector;

	class Application
	{
	public:
		Application(int resolution, bool fullscreen);
		~Application();
		void Initialize();
		void Run();
		void Shutdown() const;

		void BroadcastMessage(Message* message) const;
		
		void SetAppActive(bool active) { m_Active = active; }
		GLFWwindow* GetWindow() const { return m_mainwindow; }
		static Application* GetAppInstance();
		static Application* GetAppInstance(int resolution, bool fullscreen);

		template<typename T>
		T* getSystem()
		{
			for (size_t i = 0; i < m_systemList.size(); ++i)
			{
				if (m_systemList[i].name == typeid(T).raw_name())
				{
					return static_cast<T*>(m_systemList[i].pSystem);
				}
			}
			return nullptr;
		}
		template<typename T>
		void addSystem(T* system)
		{
			SystemData newData;
			newData.name = typeid(T).raw_name();
			newData.pSystem = system;
			m_systemList.push_back(newData);
		}

		/*void deleteGraphic()
		{
			
			for (unsigned i = 0;)
			{
				if (typeid(GraphicsRenderer).raw_name() == it.name)
				{
					it.pSystem->Shutdown();
					delete it.pSystem;
					it.pSystem = new GraphicsRenderer();
				}
			}
		}*/

		ResolutionType GetPrevResolution() const;

		void SetCurrentResolution(ResolutionType);

		void ToggleFullscreen();


		int GetPrevWinWidthSize();
		int GetPrevWinHeightSize();


		//template<typename T>
		//T* getSystem()
		//{
		//	return dynamic_cast<T*>(m_systemList[typeid(T).raw_name()]);
		//}
		//template<typename T>
		//void addSystem(T* system)
		//{
		//	m_systemList.emplace(typeid(T).raw_name(), system);
		//}
		bool m_displayMode;
		bool m_fullScreen;

		ResolutionType m_prevResolution;
		ResolutionType m_currentResolution;

		//std::array<std::tuple<int, int>, 4> m_resolution;
		std::vector<std::pair<int, int>> m_resolution;

		void Init();
		void Recreate();

		unsigned long frame_count = 0;
		double last_time, current_time;

	private:
		GLFWvidmode *m_mode;
		GLFWmonitor* monitor;
		GLFWwindow  *m_window, *m_mainwindow;
		SystemVector m_systemList;
		StateManager* m_stateManager;
		bool m_Active;
		float m_currentTime;
		float m_fullScreenWidth;
		float m_fullScreenHeight;

		void FocusWindow(void);

		//singleton
		static Application* m_appInstance;
		Application();
	};

}
