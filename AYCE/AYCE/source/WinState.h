/*****************************************************************************/
/*!
\file   WinState.h
\author Dokyoom Kim
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
source file for win state
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"
#include "State.h"
#include "Application.h"
#include "Particle.h"

namespace VK
{
	class AudioSystem;
	class InputSystem;
	class EntitySystem;
	class StateSystem;
	enum WINSTATE;

	enum WINSELECTION {
		BACKTOMENU,
		BACKTOMAPSELECT,
		QUIT_GAME
	};

}


class WinState : public VK::State
{
public:
	WinState();
	~WinState();

	void Enter() override;
	void Update(float dt) override;
	void Exit() override;

	void particle(int numbers);
	void CameraWorks(float dt);
	void ShowWhatTodo(bool truefalse);
	void ChangeSelectionBox(int selection, int selctionGo);
private:
	VK::Application* m_app = VK::Application::GetAppInstance();
	VK::AudioSystem* p_audioSystem;
	VK::InputSystem* p_inputSystem;
	VK::StateSystem* p_stateSystem;
	VK::EntitySystem* p_entitySystem;

	VK::WINSTATE m_winState;
	VK::WINSELECTION m_Winselect;
	
	float m_winStateStartTime = 0;
	VK::Particle* p_particle;

	bool m_isQuit;
	int m_Quit;

	bool m_cameraworks;

	void SetWinner(VK::WINSTATE state);
};
