/*****************************************************************************/
/*!
\file   State.h
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
#include "precompiled.h"
namespace VK
{
	class State
	{
	public:
		virtual ~State() {};

		virtual void Enter() = 0;
		virtual void Update(float dt) = 0;
		virtual void Exit() = 0;
	};
}