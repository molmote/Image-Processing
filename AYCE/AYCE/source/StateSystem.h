/*****************************************************************************/
/*!
\file   StateSystem.h
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
#pragma once
#define MAX_PAUSE 10
#include "precompiled.h"
#include "System.h"
#include "State.h"
#include <vector>
#include <list>
namespace VK
{
	enum STATE
	{
		STATE_GAME,
		STATE_MENU,
		STATE_OPTION,
		STATE_EDITOR,
		STATE_CREDITS,
		STATE_PAUSE,
		STATE_HOWTOPLAY,
		STATE_MAPSELECT,
		STATE_LOGO,
		STATE_WIN
	};

	enum MAPSTATE
	{
		MAP_1,
		MAP_2,
		MAP_3,
		MAP_MAX
	};

	enum WINSTATE
	{
		NONE,
		PLAYER1_WIN,
		PLAYER2_WIN,
		DRAW,
	};


	typedef std::list<STATE> PauseList;
	typedef std::vector<State*> StateList;

	class StateSystem : public System
	{
	public:
		StateSystem();
		virtual ~StateSystem();

		void Init() override;
		void Update(float dt) override;
		void Shutdown() override;
		void ProcessMessage(Message* message) override;


		void AddState(State* pState);
		void SetNextState(const STATE &state);
		STATE GetCurrentState() const;
		STATE GetPreviousState() const;
		STATE GetNextState() const;

		void PauseAndSetNext(const STATE &state);
		void Resume();
		void SetNextStateFromPause(const STATE &state);

		MAPSTATE GetMapState() const;
		void SetMapState(MAPSTATE map);
		WINSTATE GetWinState() const;
		void SetWinState(WINSTATE win);

		bool GetFirstTimePlaying(void) const;
		void SetFirstTimePlaying(bool first);
		StateList m_stateList;
		void SetPauseFalse(void)
		{
			
		}

	private:
		STATE currentState;
		STATE previousState;
		STATE nextState;
		MAPSTATE m_mapState;
		WINSTATE m_winState;

		PauseList m_pauseList;
		bool m_restart;
		bool m_paused[MAX_PAUSE];
		bool m_resume[MAX_PAUSE];
		unsigned int m_pauseCount;

		bool m_firstTimePlaying = true;

		void SetStartingState(const STATE &state);

	};

}