/*****************************************************************************/
/*!
\file   Picking.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Picking features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "precompiled.h"
#include <unordered_map>

namespace Picking
{
	struct PickableInstance
	{
		glm::vec3 mPosition;		//!< Position
		glm::vec3 mScale;		//!< Scale
		glm::vec3 mRotation;		//!< Rotation (in radian)
		AABB mAABB;				//!< Untransformed AABB
	};

	// Choose picked
	Hash Pick(const std::unordered_map< Hash, PickableInstance > & pickables,	//!< Pickables
		const glm::mat4 & world,											//!< From model to world
		const glm::mat4 & view,											//!< From world to view
		const glm::mat4 & perspective,										//!< From view to perspective
		const glm::vec2 & mousepos,										//!< Mouse click position
		const glm::vec2 & screenDimension);								//!< Window dimension

																		// Get click point on near and far plane
	void GetPickingInfo(const glm::mat4 & world,									//!< From model to  world
		const glm::mat4 & view,									//!< From world to view
		const glm::mat4 & perspective,							//!< From view to perspective
		const glm::vec2 & mousepos,								//!< Mouse click position
		const glm::vec2 & screenDimension,						//!< Window dimension
		glm::vec3 & clickNear, glm::vec3 & clickFar);

	// Get sphere radius in view frame (For picking in view)
	float GetPickingRadius(const glm::vec3 & scale,
		const glm::mat4 & view);

	// Time of intersection
	float RayOBBIntersection(const glm::vec3& ray, const glm::vec3& clickpoint,
		const glm::vec3& pos, const glm::vec3& scale,
		const glm::vec3 axis[3]);

	float RayPlaneIntersection(const glm::vec3 & ray, const glm::vec3 & clickpoint,
		const glm::vec3 & pos,
		const glm::vec3 & normal);

	float RayPlaneEdgeIntersection(const glm::vec3 & ray, const glm::vec3 & clickpoint,
		const glm::vec3 & pos, float radius,
		const glm::vec3 & normal);

	float RaySphereIntersection(const glm::vec3 & viewpoint, const glm::vec3 & clickpoint,
		const glm::vec3 & center, float radius);
}