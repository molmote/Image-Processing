/*****************************************************************************/
/*!
\file   StateSystem.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for StateSystem features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "StateSystem.h"
#include "Game.h"
#include "PauseState.h"
#include "Menu.h"
#include "Option.h"
#include "LevelEditor.h"
#include "Credits.h"
#include "Howtoplay.h"
#include "MapSelect.h"
#include "Logo.h"
#include "WinState.h"

namespace VK 
{


	StateSystem::StateSystem()
	{
		SetStartingState(STATE_LOGO);
		AddState(new Game);
		AddState(new Menu);
		AddState(new Option);
		AddState(new LevelEditor);
		AddState(new Credits);
		AddState(new PauseState);
		AddState(new HowtoPlay);
		AddState(new MapSelect);
		AddState(new Logo);
		AddState(new WinState);

		m_restart = false;
		m_pauseCount = 0;
		m_mapState = MAP_1;
		m_winState = NONE;
		for (int i = 0; i < MAX_PAUSE; i++)
		{
			m_resume[i] = false;
			m_paused[i] = false;
		}
	}


	StateSystem::~StateSystem()
	{
		for (auto& it : m_stateList)
		{
			delete it;
		}
	}


	void StateSystem::Init()
	{
		START();	

		m_stateList[currentState]->Enter();

		LOG("Initialize All the states in the state system");
	}
	void StateSystem::Update(float dt)
	{



		if (currentState != nextState || m_restart || m_resume[m_pauseCount])
		{
			if (!m_paused[m_pauseCount])
				m_stateList[currentState]->Exit();
			if (!m_resume[m_pauseCount])
				m_stateList[nextState]->Enter();
			else
			{
				m_resume[m_pauseCount] = false;
			}


			previousState = currentState;
			currentState = nextState;
			m_restart = false;
		}
		
		m_stateList[currentState]->Update(dt);

		if (m_resume[m_pauseCount])
		{
			nextState = m_pauseList.back();
			m_pauseList.pop_back();
		}

	}
	void StateSystem::Shutdown()
	{
		m_stateList[currentState]->Exit();
	}


	void  StateSystem::AddState(State* pState)
	{
		m_stateList.push_back(pState);
	}

	void StateSystem::SetStartingState(const STATE &state)
	{
		previousState = state;
		currentState = state;
		nextState = state;
	}

	void StateSystem::SetNextState(const STATE &state)
	{
		nextState = state;
	}
	STATE StateSystem::GetCurrentState() const
	{
		return currentState;
	}
	STATE StateSystem::GetPreviousState() const
	{
		return previousState;
	}
	STATE StateSystem::GetNextState() const
	{
		return nextState;
	}


	void StateSystem::PauseAndSetNext(const STATE &state)
	{

		++m_pauseCount;
		m_paused[m_pauseCount] = true;
		m_pauseList.push_back(currentState);

		nextState = STATE_PAUSE;
	}
	void StateSystem::Resume()
	{

		m_resume[m_pauseCount] = true;
		m_paused[m_pauseCount] = false;
	}
	void StateSystem::SetNextStateFromPause(const STATE &state)
	{

		for (int i = 0; i < MAX_PAUSE; i++)
		{
			m_resume[i] = false;
			m_paused[i] = false;
		}

		m_pauseCount = 0;

		m_stateList[m_pauseList.back()]->Exit();

		nextState = state;

		if (!m_pauseList.empty())
		{
			for (PauseList::iterator it = m_pauseList.begin(); it != m_pauseList.end(); ++it)
			{
				m_stateList[*it]->Exit();

			}
			m_pauseList.clear();
		}
	}

	MAPSTATE StateSystem::GetMapState() const
	{
		return m_mapState;
	}
	void StateSystem::SetMapState(MAPSTATE map)
	{
		m_mapState = map;
	}

	WINSTATE StateSystem::GetWinState() const
	{
		return m_winState;
	}
	void StateSystem::SetWinState(WINSTATE win)
	{
		m_winState = win;
	}
	void StateSystem::ProcessMessage(Message* message)
	{

	}
	bool StateSystem::GetFirstTimePlaying(void) const
	{
		return m_firstTimePlaying;
	}
	void StateSystem::SetFirstTimePlaying(bool first)
	{
		m_firstTimePlaying = first;
	}
}