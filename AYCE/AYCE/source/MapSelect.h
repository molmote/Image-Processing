/*****************************************************************************/
/*!
\file   MapSelect.h
\author Dokyoom Kim
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
source file for mapselect
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "State.h"
#include "Application.h"
#include "precompiled.h"

namespace VK
{
	class AudioSystem;
	class InputSystem;
	class EntitySystem;
	class StateSystem;
	enum MAPSTATE;
}


class MapSelect : public VK::State
{
public:
	MapSelect();
	~MapSelect();

	void Enter() override;
	void Update(float dt) override;
	void Exit() override;

	
private:
	int m_mapState;

	VK::Application* m_app = VK::Application::GetAppInstance();
	VK::AudioSystem* p_audioSystem;
	VK::InputSystem* p_inputSystem;
	VK::StateSystem* p_stateSystem;
	VK::EntitySystem* p_entitySystem;

};
