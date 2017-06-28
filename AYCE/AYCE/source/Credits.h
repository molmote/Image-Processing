/*****************************************************************************/
/*!
\file   Credits.h
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Credits features
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
}

class Credits : public VK::State
{
public:
	Credits();
	~Credits();

	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
	void CameraWorks(float dt) const;
private:
	VK::Application* m_app = VK::Application::GetAppInstance();
	VK::InputSystem* p_inputSystem;
	VK::StateSystem* p_stateSystem;
	VK::EntitySystem* p_entitySystem;
	VK::AudioSystem* p_audioSystem;

};

