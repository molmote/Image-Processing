/*****************************************************************************/
/*!
\file   Howtoplay.h
\author DoKyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
This is the source file for Howtoplay state of Game play. When player enter
into pause state, and selection box goes to How to play section, the How to
play message came out.
Copyright 2017, Digipen Institute of Technology
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
	enum STATE;
}

class HowtoPlay : public VK::State
{
public:
	HowtoPlay();
	~HowtoPlay();

	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
private:
	VK::Application* m_app = VK::Application::GetAppInstance();
	VK::InputSystem* p_inputSystem;
	VK::StateSystem* p_stateSystem;
	VK::EntitySystem* p_entitySystem;
	VK::STATE prev;

	float m_howtoplayTime = 0;

};

