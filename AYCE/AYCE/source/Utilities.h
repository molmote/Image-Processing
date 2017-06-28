/*****************************************************************************/
/*!
\file   Utilities.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Mesh features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "precompiled.h"


namespace VK
{
	class Entity;
}

namespace Utilities
{
	float RandMinMax(const float Min, const float Max);
	int RandMinMax(const int Min, const int Max);
	float RayAabb(const glm::vec3& rayStart, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& t);
	VK::Entity* Picking(void);
}