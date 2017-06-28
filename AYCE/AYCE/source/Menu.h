/*****************************************************************************/
/*!
\file   Menu.h
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Menu features
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
	class PhysicsSystem;
	class ImguiManager;
	class StateSystem;
	class Particle;
	class GameLogic;

}
class Menu : public VK::State
{
public:
	Menu();
	~Menu();

	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
	void CameraMove(int Key);
	void CameraWorks(int Target, float dt);
	void Particle();
	void RotateParticle(int camera);
private:
	std::string StateName;
	VK::Application* m_app = VK::Application::GetAppInstance();
	VK::AudioSystem* p_audioSystem;
	VK::InputSystem* p_inputSystem;
	VK::EntitySystem* p_entitySystem;
	VK::PhysicsSystem* p_physicsSystem;
	VK::ImguiManager* p_imguiSystem;
	VK::StateSystem* p_stateSystem;
	VK::Particle* p_particle;
	VK::GameLogic* p_gameLogic;

	bool m_isQuit;
	int m_Quit;
	int particleCounter;
};

