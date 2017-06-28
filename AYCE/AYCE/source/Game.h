/*****************************************************************************/
/*!
\file   Game.h
\author Hyeongjin Park, Dokyoom Kim
\par    email: 
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Game features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

#include "State.h"
#include"Application.h"
#include "Entity.h"
#include "CharacterMove.h"

namespace VK
{
	class AudioSystem;
	class InputSystem;
	class EntitySystem;
	class PhysicsSystem;
	class ImguiManager;
	class StateSystem;
	class GameLogic;
	enum MAPSTATE;

enum ObstacleType
{
	O_TYPE1,
	O_TYPE2,
	O_TYPE3,
	O_TYPE4
};

	enum class CharMoveFlag
	{
		CharMoveFlag_First = 0,
		CharMoveFlag_UP = CharMoveFlag_First,
		CharMoveFlag_RIGHT,
		CharMoveFlag_DOWN,
		CharMoveFlag_LEFT,
		CharMoveFlag_Size,
	};
class Game : public VK::State
{
public:
	Game();
	~Game();

	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
	void HPbarScale() const; 
	void WinLostCondition();
	void CameraMovement(float dt);
	void CharMovement(VK::Entity* entity, VK::CharacterMove* p_CM, float dt, CharMoveFlag move_flag);
	void CreateSnowBall(VK::Entity* entity);
	bool isOverlap(Entity* entity) const;
private:
	double m_startx;
	double m_starty;
	bool check;
	int audio_id;
	bool button;
	bool editor_button;
	bool m_Player1Invincible, m_Player2Invincible;
	bool m_Win;
	float m_startTime = 0;
	float m_snowballTime = 0;
	float m_windTime = 0;
	int countObstacle = 0;
	bool checkToStart = false;
	bool checkObstacleType = true;
	int random = 0;
	
	VK::MAPSTATE m_map;

	void CreateObstacle(ObstacleType type);

	//Factory* m_pFactory;

	VK::Application* m_app = VK::Application::GetAppInstance();
	VK::AudioSystem* p_audioSystem;
	VK::InputSystem* p_inputSystem;
	VK::EntitySystem* p_entitySystem;
	VK::PhysicsSystem* p_physicsSystem;
	VK::ImguiManager* p_imguiSystem;
	VK::StateSystem* p_stateSystem;
	VK::GameLogic* p_gameLogic;

	int m_snowIDAuto = 0;
	bool p1_autoplay = false;
	bool p2_autoplay = false;

	Entity* player1 = nullptr;
	Entity* player2 = nullptr;

	double p1_last_time = 0., p1_current_time = 0.;
	double p2_last_time = 0., p2_current_time = 0.;

	float p1_cooltime = 0.f, p2_cooltime = 0.f;

	int p1_direction = 0, p2_direction = 0;
};
}//namespace VK;
