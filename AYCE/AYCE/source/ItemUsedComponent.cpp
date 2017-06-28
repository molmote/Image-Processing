
/*****************************************************************************/
/*!
\file   ItemUsedComponent.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   04/12/2017
\brief
This is the source file for HoleComponent
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "ItemUsedComponent.h"
#include "Entity.h"

namespace VK
{

	ItemUsedComponent::ItemUsedComponent()
	{
		m_used = false;
	}
	ItemUsedComponent::~ItemUsedComponent()
	{

	}

	bool ItemUsedComponent::GetItemUsed(void)
	{
		return m_used;
	}
	void ItemUsedComponent::SetItemUsed(bool use)
	{
		m_used = use;
	}


	ItemType ItemUsedComponent::GetItemType(void)
	{
		return m_itemType;
	}
	void ItemUsedComponent::SetItemType(ItemType type)
	{
		m_itemType = type;
	}
}