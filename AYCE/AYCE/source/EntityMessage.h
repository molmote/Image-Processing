/*****************************************************************************/
/*!
\file   EntityMessage.h
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for EntityMessage features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"
namespace VK
{
	enum EMessageType
	{
		HI
	};

	class EMessage
	{
	public:
		virtual ~EMessage() {};
		EMessage(EMessageType type) : m_eMessageType(type) {};
		EMessageType m_eMessageType;
	};
}