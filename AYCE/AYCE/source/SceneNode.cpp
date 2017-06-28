/*****************************************************************************/
/*!
\file   SceneNode.cpp
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for SceneNode features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "SceneNode.h"

namespace VK
{
	SceneNode::SceneNode() :
		transform()
	{
	}


	SceneNode::~SceneNode()
	{
	}

	TransformComponent& SceneNode::getTransform() {
		return transform;
	}
}