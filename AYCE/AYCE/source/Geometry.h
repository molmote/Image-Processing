/*****************************************************************************/
/*!
\file   Geometry.h
\author Hyeongjin Park
\par    email: park.hyungjin\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Geometry features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

#include "math.h"

namespace VK
{
	class Plane
	{
	public:
		Plane();
		// Create the plane from a triangle
		Plane(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
		// Create the plane from a point and a normal
		Plane(const glm::vec3& normal, const glm::vec3& point);

		// Create a plane from a triangle. The plane's normal should be normalized.
		void Set(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
		// Create a plane from a point and a normal. The plane's normal should be normalized.
		void Set(const glm::vec3& normal, const glm::vec3& point);

		// Get the normal of this plane.
		glm::vec3 GetNormal() const;
		// Get the 'd' value (the distance from the origin) which can be computed as Dot(origin - pointOnPlane, normal)
		float GetDistance() const;


		glm::vec4 mData;
	};

	namespace IntersectionType
	{
		enum Type { Coplanar = 0, Outside, Inside, Overlaps, NotImplemented };
		static const char* Names[] = { "Coplanar", "Outside", "Inside", "Overlaps", "NotImplemented" };
	}

	namespace Geometry
	{
		// Helper function that you should likely use in several plane intersection functions. Also required for unit-tests.
		glm::vec3 ProjectPointOnPlane(const glm::vec3& point, const glm::vec3& normal, float planeDistance);

		// Compute the barycentric coordinates of a point to a line. Returns false if the line is degenerate or the passed in point is outside the line.
		// The given epsilon should be used to expand the line before checking if the point is outside (use it to expand the range of the barycentric coordinate checks).
		// Note: The barycentric coordinates should still be filled out when the point is outside the line.
		bool BarycentricCoordinates(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b,
			float& u, float& v, float epsilon = 0.0f);

		// Compute the barycentric coordinates of a point to a triangle. Returns false if the triangle is degenerate or the passed in point is outside the triangle.
		// The given epsilon should be used to expand the triangle before checking if the point is outside the triangle (use it to expand the range of the barycentric coordinate checks).
		// Note: The barycentric coordinates should still be filled out when the point is outside the triangle.
		bool BarycentricCoordinates(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
			float& u, float& v, float& w, float epsilon = 0.0f);

		//--------------------------------------------------------------------------------------------------------------------
		// Point Tests
		//--------------------------------------------------------------------------------------------------------------------

		// Classify a point with respect to a plane. This should return Inside, Outside, or Coplanar (in the event the the point is within epsilon distance from the plane).
		IntersectionType::Type PointPlane(const glm::vec3& point, const glm::vec4& plane, float epsilon);

		// Determine if the given point is within the sphere. Remember, the surface of the sphere is considered part of the sphere.
		bool PointSphere(const glm::vec3& point, const glm::vec3& sphereCenter, float sphereRadius);

		// Determine if the given point is within the aabb. Remember, the surface of the aabb is considered part of the aabb.
		bool PointAabb(const glm::vec3& point, const glm::vec3& aabbMin, const glm::vec3& aabbMax);

		//--------------------------------------------------------------------------------------------------------------------
		// Ray Tests
		//--------------------------------------------------------------------------------------------------------------------

		// Ray vs. plane where the plane is represented by the general plane
		// equation ax + by + cz + d = 0 (so the plane parameters are [a, b, c, d]).
		// Epsilon is use to check if the ray is close enough to parallel to the plane; in this case return false.
		bool RayPlane(const glm::vec3& rayStart, const glm::vec3& rayDir,
			const glm::vec4& plane, float& t, float epsilon = 0.0001f);

		// Check if a ray hits a triangle. This should return false if the ray doesn't hit the triangle.
		// If the ray does hit the triangle the t-value of where the ray hits the triangle should be filled out.
		// Note: Use the triExpansionEpsilon to expand the triangle before checking for containment (in the barycentric coordinates test).
		// This becomes useful and necessary later to deal with edge cases in a mesh (e.g. in the bsp tree).
		bool RayTriangle(const glm::vec3& rayStart, const glm::vec3& rayDir,
			const glm::vec3& triP0, const glm::vec3& triP1, const glm::vec3& triP2,
			float& t, float triExpansionEpsilon);

		// Check if a ray hits a sphere. If the ray does hit the sphere then t should be filled out with the first time of impact.
		// If the ray starts inside the sphere then t should be 0. Note: t should never be set to a negative value if this function returns true!
		bool RaySphere(const glm::vec3& rayStart, const glm::vec3& rayDir,
			const glm::vec3& sphereCenter, float sphereRadius,
			float& t);

		// Check if a ray hits an aabb. If the ray does hit the aabb then t should be filled out with the first time of impact.
		// If the ray starts inside the aabb then t should be 0. Note: t should never be set to a negative value if this function returns true!
		// Do not call RayPlane here (it's inefficient)!
		bool RayAabb(const glm::vec3& rayStart, const glm::vec3& rayDir,
			const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& t);

		//--------------------------------------------------------------------------------------------------------------------
		// Plane Tests
		//--------------------------------------------------------------------------------------------------------------------

		// Classify a triangle against a plane. This should return Coplanar only if all points are within epsilon distance from the plane.
		IntersectionType::Type PlaneTriangle(const glm::vec4& plane,
			const glm::vec3& triP0, const glm::vec3& triP1, const glm::vec3& triP2,
			float epsilon);

		// Classify a sphere against a plane. This should return Overlaps if the sphere hits the plane. No check for Coplanar is needed.
		IntersectionType::Type PlaneSphere(const glm::vec4& plane,
			const glm::vec3& sphereCenter, float sphereRadius);

		// Classify an aabb against a plane. This should return Overlaps if the aabb hits the plane. Do not check all 8 points of the aabb against the plane!
		// You must use the 2 point method described in class (the furthest positive and negative point method). No check for Coplanar is needed.
		IntersectionType::Type PlaneAabb(const glm::vec4& plane,
			const glm::vec3& aabbMin, const glm::vec3& aabbMax);

		//--------------------------------------------------------------------------------------------------------------------
		// Frustum Tests. Note: You cannot determine that a shape Overlaps if one plane of the frustum classifies as Overlaps.
		// The shape could still be outside another plane!! Also, Coplanar checks are not necessary.
		// Finally, the lastAxis variable is not needed for the first assignment, but will be needed in assignment 3.
		// The last axis should be filled out with an axis that causes the test to return Outside (if there is one).
		// This should also be used as the first axis you test so that temporal coherence can be used to reject after only 1 plane test the next frame.
		//--------------------------------------------------------------------------------------------------------------------

		// Classify a triangle against a frustum. The given epsilon should be used when classifying the triangle points against the plane.
		IntersectionType::Type FrustumTriangle(const glm::vec4 planes[6],
			const glm::vec3& triP0, const glm::vec3& triP1, const glm::vec3& triP2,
			float epsilon);


		IntersectionType::Type FrustumSphere(const glm::vec4 planes[6],
			const glm::vec3& sphereCenter, float sphereRadius, size_t& lastAxis);

		IntersectionType::Type FrustumAabb(const glm::vec4 planes[6],
			const glm::vec3& aabbMin, const glm::vec3& aabbMax, size_t& lastAxis);

		//--------------------------------------------------------------------------------------------------------------------
		// Simple primitive tests
		//--------------------------------------------------------------------------------------------------------------------
		bool SphereSphere(const glm::vec3& sphereCenter0, float sphereRadius0,
			const glm::vec3& sphereCenter1, float sphereRadius1);

		bool AabbAabb(const glm::vec3& aabbMin0, const glm::vec3& aabbMax0,
			const glm::vec3& aabbMin1, const glm::vec3& aabbMax1);
	}
	

}
