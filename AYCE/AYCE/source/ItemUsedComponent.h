/*****************************************************************************/
/*!
\file   ItemUsedComponent.h
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
This is the interface file for SnowballMove features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "Component.h"
#include "precompiled.h"

namespace VK
{
	enum ItemType;

	class ItemUsedComponent : public Component
	{
	public:
		ItemUsedComponent();
		virtual ~ItemUsedComponent();

		bool GetItemUsed(void);
		void SetItemUsed(bool use);
		ItemType GetItemType(void);
		void SetItemType(ItemType type);
	private:
		bool m_used;
		ItemType m_itemType;
	};
}