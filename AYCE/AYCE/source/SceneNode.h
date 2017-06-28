/*****************************************************************************/
/*!
\file   SceneNode.h
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
#pragma once

#include "precompiled.h"
#include "TransformComponent.h"
//#include "Device.h"
//#include "GraphicsRenderer.h"
#include "Mesh.h"

namespace VK
{

	class SceneNode
	{
	public:
		SceneNode();
		virtual ~SceneNode();
		TransformComponent& getTransform();

	private:
		TransformComponent transform;
		//Mesh* mesh;
	};
}