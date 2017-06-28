/*****************************************************************************/
/*!
\file   System.h
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for System features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
//#include "precompiled.h"
#include "Message.h"

namespace VK
{
	class System
	{
	public:
		virtual ~System() {}
		virtual void Init() = 0;
		virtual void Update(float dt) = 0;
		virtual void Shutdown() = 0;
		virtual void ProcessMessage(Message* message) {};
	};
}