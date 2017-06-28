/*****************************************************************************/
/*!
\file   PauseState.h
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for PauseState features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

#include "State.h"
#include "Application.h"

namespace VK
{
	class AudioSystem;
	class InputSystem;
	class EntitySystem;
	class StateSystem;
}

enum PAUSESTATE {
	RESUME,
	GAMERESTART,
	OPTION,
	BTM,
	HTP,
	CREDITS,
	QUIT
};

class PauseState : public VK::State
{
public:
	PauseState();
	virtual ~PauseState();


	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
	void ChangeSelectionBox(int selection, int selctionGo);
	//void ShowHowToPlay(bool truefalse);
private:
	int m_pauseState;
	VK::Application* m_app = VK::Application::GetAppInstance();
	VK::AudioSystem* p_audioSystem;
	VK::InputSystem* p_inputSystem;
	VK::EntitySystem* p_entitySystem;
	VK::StateSystem* p_stateSystem;
	double xstart;
	double ystart;
	bool check;

	bool m_isQuit;
	int m_Quit;
};