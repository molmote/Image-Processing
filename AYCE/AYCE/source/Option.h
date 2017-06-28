/*****************************************************************************/
/*!
\file   Option.h
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   28/05/2015
\brief
This is the interface file for Option features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

#include "State.h"
#include "Application.h"

namespace VK
{
	class InputSystem;
	class StateSystem;
	class EntitySystem;
	class AudioSystem;
	
	enum class OptionState {
		FirstFLAG,
		Back = 0,
		Sound,
		Resolution,
		Quit_,
		LASTFLAG
	};
	enum STATE;

	class Option : public VK::State
	{
	public:
		Option();
		~Option();

		void Enter() override;
		void Update(float dt) override;
		void Exit() override;
		void ChangeSelectionBox(OptionState selection, OptionState selctionGo);
		void StateofOption(OptionState state);
		void ChangeSound();
		void ShowupResolution(ResolutionType resolution);
		OptionState m_OptionState;

	private:
		VK::Application* m_app = VK::Application::GetAppInstance();
		VK::InputSystem* p_inputSystem;
		VK::StateSystem* p_stateSystem;
		VK::EntitySystem* p_entitySystem;
		VK::AudioSystem* p_audioSystem;
		float m_volume;
		bool m_mute;
		bool m_fullScreen;
		ResolutionType m_resolution;
		bool m_isQuit;
		int m_Quit;
		glm::vec3 m_oriSoundBarSize;
		VK::STATE prev;

		//std::deque<OptionState> m_OptionState;
	};
} // namespace VK


