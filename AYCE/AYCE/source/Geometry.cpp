/*****************************************************************************/
/*!
\file   Geometry.cpp
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
#include "Geometry.h"

namespace VK {
	Plane::Plane()
	{
		mData = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	Plane::Plane(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
	{
		Set(p0, p1, p2);
	}

	Plane::Plane(const glm::vec3& normal, const glm::vec3& point)
	{
		Set(normal, point);
	}

	void Plane::Set(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
	{
		glm::vec3 Cross = glm::cross(p1 - p0, p2 - p0);
		if (Cross.x == 0 && Cross.y == 0 && Cross.z == 0)
		{
			mData.x = Cross.x;
			mData.y = Cross.y;
			mData.z = Cross.z;
			mData.w = glm::dot(Cross,p0);
			return;
		}
		glm::normalize(Cross);
		mData.x = Cross.x;
		mData.y = Cross.y;
		mData.z = Cross.z;
		mData.w = glm::dot(Cross, p0);
	}

	void Plane::Set(const glm::vec3& normal, const glm::vec3& point)
	{
		if (normal.x == 0 && normal.y == 0 && normal.z == 0)
		{
			mData.x = normal.x;
			mData.y = normal.y;
			mData.z = normal.z;
			mData.w = glm::dot(normal,point);
			return;
		}
		glm::vec3 Normalized = glm::normalize(normal);
		mData.x = Normalized.x;
		mData.y = Normalized.y;
		mData.z = Normalized.z;
		mData.w = glm::dot(Normalized,point);
	}

	glm::vec3 Plane::GetNormal() const
	{
		return glm::vec3(mData.x, mData.y, mData.z);
	}

	float Plane::GetDistance() const
	{
		return mData.w;
	}
	namespace Geometry
	{
		glm::vec3 ProjectPointOnPlane(const glm::vec3& point, const glm::vec3& normal, float planeDistance)
		{
			// calculate the normal length in case of 0
			float lengthSq = glm::length(normal)*glm::length(normal);
			if (lengthSq == 0.0f)
				return point;
			// calculate the distance (with out abs)
			float distancePointAndPlane = (glm::dot(normal, point) - planeDistance) / lengthSq;
			// the projection point would be origin point add distance(directional info) * normal
			return glm::vec3(point - normal*distancePointAndPlane);
		}

		bool BarycentricCoordinates(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b,
			float& u, float& v, float epsilon)
		{
			// this is not good way to solve
			// get all distance
			bool TrueFalse;
			float distanceAB = glm::distance(a, b);
			float distancePA = glm::distance(point, a);
			float distancePB = glm::distance(point, b);

			// in case disntace == 0 // denominator
			if (distanceAB == 0)
				return false;
			// calculate uv
			v = distancePA / distanceAB;
			u = distancePB / distanceAB;

			if (1 - epsilon <= u + v && u + v <= 1 + epsilon)
				TrueFalse = true;
			else
				TrueFalse = false;

			// because I already calculate u,v I have to figure it out 
			// if one of them bigger than 1 or not(less than 0)
			if (v > 1)
				u = 1 - v;
			if (u > 1)
				v = 1 - u;

			if (TrueFalse)
				return true;
			else
				return false;
		}

		bool BarycentricCoordinates(const glm::vec3& point, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
			float& u, float& v, float& w, float epsilon)
		{
			// this good way to solve, only calulate two of u v w
			// calculate cross product of ABC
			glm::vec3 cABC = glm::cross(b - a, c - a);
			// get area of cABC
			float AreaABC = glm::dot(cABC, cABC);
			// in case of dominator == 0
			if (AreaABC == 0)
				return false;
			// calculate cross product of PCA, PBC for U,V
			glm::vec3 cPCA = glm::cross(c - point, a - point);
			glm::vec3 cPBC = glm::cross(b - point, c - point);
			// calculate area care about the sign
			float AreaPCA = glm::dot(cPCA, cABC);
			float AreaPBC = glm::dot(cPBC, cABC);
			// get u v area dividing by AreaABC
			u = AreaPBC / AreaABC;
			v = AreaPCA / AreaABC;
			w = 1 - v - u;

			float PositiveThick = 1 + epsilon;
			float NegativeThick = 0 - epsilon;
			// in case u,v,w get outside of the range
			if (u > PositiveThick || u < NegativeThick
				|| v > PositiveThick || v < NegativeThick
				|| w > PositiveThick || w < NegativeThick)
				return false;
			else
				return true;
		}

		IntersectionType::Type PointPlane(const glm::vec3& point, const glm::vec4& plane, float epsilon)
		{
			glm::vec3 planeNormal(plane.x, plane.y, plane.z);
			// calculate length of point and plane with direction( directional, same as point projection on plane )
			float distancePointAndPlane = (glm::dot(planeNormal, point) - plane.w) / glm::length(planeNormal);
			// if distance is less than -epsilon ( under plane )
			if (-epsilon > distancePointAndPlane)
				return IntersectionType::Outside;
			// if distance is bigger than epsilon ( down plane )
			if (distancePointAndPlane > epsilon)
				return IntersectionType::Inside;
			// if distnace is between both positive and negative epsilon
			if (-epsilon < distancePointAndPlane && distancePointAndPlane < epsilon)
				return IntersectionType::Coplanar;

			// return NotImplemented if nothing handled.
			return IntersectionType::NotImplemented;

		}

		bool PointSphere(const glm::vec3& point, const glm::vec3& sphereCenter, float sphereRadius)
		{
			// calculate distance between point and sphere center
			float Distance = glm::distance(point, sphereCenter);
			// if that distance less than radius
			if (Distance <= sphereRadius)
				return true;
			else return false;

		}

		bool PointAabb(const glm::vec3& point, const glm::vec3& aabbMin, const glm::vec3& aabbMax)
		{
			// check if point is inside aabb or not
			if (point.x <= aabbMax.x && point.x >= aabbMin.x)
				if (point.y <= aabbMax.y && point.y >= aabbMin.y)
					if (point.z <= aabbMax.z && point.z >= aabbMin.z)
						return true;

			return false;
		}

		bool RayPlane(const glm::vec3& rayStart, const glm::vec3& rayDir,
			const glm::vec4& plane, float& t, float epsilon)
		{
			// get normal of plane
			glm::vec3 norm(plane.x, plane.y, plane.z);
			// get dot product of raydirection and plane normal
			float dotRayNorm = glm::dot(rayDir, norm);
			// if parallel
			if (abs(dotRayNorm) < epsilon)
				return false;
			t = (plane.w - glm::dot(rayStart, norm)) / dotRayNorm;
			return !(t < epsilon);

		}

		bool RayTriangle(const glm::vec3& rayStart, const glm::vec3& rayDir,
			const glm::vec3& triP0, const glm::vec3& triP1, const glm::vec3& triP2,
			float& t, float triExpansionEpsilon)
		{
			// make(set) plane
			Plane plane(triP0, triP1, triP2);
			// if ray plane does not work ray does not intersect with triangle
			if (!RayPlane(rayStart, rayDir, glm::vec4(plane.GetNormal().x, plane.GetNormal().y, plane.GetNormal().z, plane.GetDistance()), t, triExpansionEpsilon))
				return false;
			// if intersect, calculate ray to plane point
			glm::vec3 planePoint = rayStart + rayDir*t;
			float uf = 0.f, vf = 0.f, wf = 0.f;
			// so calculate BarycentricCoordinates(triangle) part to ray intersect with triangle.
			if (BarycentricCoordinates(planePoint, triP0, triP1, triP2, vf, uf, wf, triExpansionEpsilon))
				return true;
			return false;
		}

		bool RaySphere(const glm::vec3& rayStart, const glm::vec3& rayDir,
			const glm::vec3& sphereCenter, float sphereRadius,
			float& t)
		{
			// first check that if sphere is behind ray or not
			float RsToSc = glm::dot(rayDir, sphereCenter - rayStart);
			if (RsToSc < 0)
				return false;
			// check the if the ray starts inside of sphere or not
			if (glm::distance(rayStart, sphereCenter) < sphereRadius)
			{
				t = 0;
				return true;
			}
			// if sphere is front of ray, do the equation
			glm::vec3 K = sphereCenter - rayStart;
			float kSQ = glm::dot(K, K);
			float a = glm::dot(rayDir, rayDir);
			float b = -2 * glm::dot(K, rayDir);
			float c = kSQ - sphereRadius*sphereRadius;
			float determinant = b*b - 4 * a*c;
			if (determinant < 0)
				return false;
			else if (determinant == 0)
			{
				t = (2 * glm::dot(K, rayDir) - glm::sqrt(determinant)) / 2 * a;
				return true;
			}
			else if (determinant > 0)
			{
				t = (2 * glm::dot(K, rayDir) - glm::sqrt(determinant)) / 2 * a;
				return true;
			}
			return false;
		}

		bool RayAabb(const glm::vec3& rayStart, const glm::vec3& rayDir,
			const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& t)
		{
			// calculate tmin and tmax
			// this could be for loop but this would be faster
			double tx1 = (aabbMin.x - rayStart.x) / rayDir.x;
			double tx2 = (aabbMax.x - rayStart.x) / rayDir.x;

			double tmin = glm::min(tx1, tx2);
			double tmax = glm::max(tx1, tx2);

			double ty1 = (aabbMin.y - rayStart.y) / rayDir.y;
			double ty2 = (aabbMax.y - rayStart.y) / rayDir.y;

			tmin = glm::max(tmin, glm::min(ty1, ty2));
			tmax = glm::min(tmax, glm::max(ty1, ty2));

			double tz1 = (aabbMin.z - rayStart.z) / rayDir.z;
			double tz2 = (aabbMax.z - rayStart.z) / rayDir.z;

			tmin = glm::max(tmin, glm::min(tz1, tz2));
			tmax = glm::min(tmax, glm::max(tz1, tz2));

			// after calculation, determin wich one goes t
			// because max and Min takes double as variable, I have to use float casting
			if (tmax < 0)
				t = static_cast<float>(tmax);
			else
				t = static_cast<float>(tmin);

			// in case aabb box is behind of ray, return false
			if (tmax < 0 && tmin < 0)
				return false;
			// in case ray is inside of aabb box, makes t 0
			if (t < 0)
				t = 0;

			return tmax >= tmin;
		}

		IntersectionType::Type PlaneTriangle(const glm::vec4& plane,
			const glm::vec3& triP0, const glm::vec3& triP1, const glm::vec3& triP2,
			float epsilon)
		{
			// get plane normal and length
			glm::vec3 planeNormal(plane.x, plane.y, plane.z);
			float planeNormalLength = glm::length(planeNormal);

			glm::vec3 triPoint[3] = { triP0 ,triP1,triP2 };
			float Dis_TrPo_Plane[3];

			int checkBack = 0, checkMiddle = 0, checkFront = 0;

			for (unsigned int i = 0; i < 3; ++i)
			{
				// calculate distance between each of triangle points with plane
				Dis_TrPo_Plane[i] = (glm::dot(planeNormal, triPoint[i]) - plane.w) / planeNormalLength;
				if (Dis_TrPo_Plane[i] > epsilon) checkFront++;
				else if (Dis_TrPo_Plane[i] < -epsilon) checkBack++;
				else checkMiddle++;
			}
			// if that all distance located in front 
			if (checkFront == 3)
				return IntersectionType::Inside;
			// or other cases
			else if (checkFront == 2)
			{
				if (checkMiddle == 1)
					return IntersectionType::Inside;
				else
					return IntersectionType::Overlaps;
			}
			else if (checkFront == 1)
			{
				if (checkMiddle == 2)
					return IntersectionType::Inside;
				else
					return IntersectionType::Overlaps;
			}
			else
			{
				if (checkMiddle == 3)
					return IntersectionType::Coplanar;
				else
					return IntersectionType::Outside;
			}
		}

		IntersectionType::Type PlaneSphere(const glm::vec4& plane,
			const glm::vec3& sphereCenter, float sphereRadius)
		{
			// calculate the normal length in case of 0
			glm::vec3 normal = glm::vec3(plane.x, plane.y, plane.z);
			// calculate the distance (with out abs)
			float distancePointAndPlane = (glm::dot(normal, sphereCenter) - plane.w) / glm::length(normal);
			// check distance between plane and sphere center to determine intersection type
			if (distancePointAndPlane < -sphereRadius)
				return IntersectionType::Outside;
			else if (distancePointAndPlane > sphereRadius)
				return IntersectionType::Inside;
			else if (distancePointAndPlane >= -sphereRadius && distancePointAndPlane <= sphereRadius)
				return IntersectionType::Overlaps;
			return IntersectionType::NotImplemented;
		}

		IntersectionType::Type PlaneAabb(const glm::vec4& plane,
			const glm::vec3& aabbMin, const glm::vec3& aabbMax)
		{
			glm::vec3 planeNormal(plane.x, plane.y, plane.z);

			// define points( all aabb min max points)
			glm::vec3 DefinePoint1(aabbMax.x, aabbMin.y, aabbMin.z);
			glm::vec3 DefinePoint2(aabbMin.x, aabbMax.y, aabbMin.z);
			glm::vec3 DefinePoint3(aabbMin.x, aabbMin.y, aabbMax.z);
			glm::vec3 DefinePoint4(aabbMax.x, aabbMax.y, aabbMin.z);
			glm::vec3 DefinePoint5(aabbMax.x, aabbMin.y, aabbMax.z);
			glm::vec3 DefinePoint6(aabbMin.x, aabbMax.y, aabbMax.z);

			// calculate length of point and plane with direction( directional, same as point projection on plane )
			// check glm::dot product of all points
			float disDPM = (glm::dot(planeNormal, aabbMin) - plane.w) / glm::length(planeNormal);
			float disDPX = (glm::dot(planeNormal, aabbMax) - plane.w) / glm::length(planeNormal);
			float disDP1 = (glm::dot(planeNormal, DefinePoint1) - plane.w) / glm::length(planeNormal);
			float disDP2 = (glm::dot(planeNormal, DefinePoint2) - plane.w) / glm::length(planeNormal);
			float disDP3 = (glm::dot(planeNormal, DefinePoint3) - plane.w) / glm::length(planeNormal);
			float disDP4 = (glm::dot(planeNormal, DefinePoint4) - plane.w) / glm::length(planeNormal);
			float disDP5 = (glm::dot(planeNormal, DefinePoint5) - plane.w) / glm::length(planeNormal);
			float disDP6 = (glm::dot(planeNormal, DefinePoint6) - plane.w) / glm::length(planeNormal);

			// if all glm::dot products are less than 0 (opposit side of plane)
			if (0 >= disDPM && 0 >= disDPX && 0 >= disDP1 && 0 >= disDP2
				&& 0 >= disDP3 && 0 >= disDP4 && 0 >= disDP5 && 0 >= disDP6)
				return IntersectionType::Outside;
			// if all glm::dot products are bigger than 0 (front side of plane)
			else if (0 <= disDPM && 0 <= disDPX && 0 <= disDP1 && 0 <= disDP2
				&& 0 <= disDP3 && 0 <= disDP4 && 0 <= disDP5 && 0 <= disDP6)
				return IntersectionType::Inside;
			// if distnace is between both positive and negative epsilon
			else
				return IntersectionType::Overlaps;

			// return NotImplemented if nothing handled.
			//return IntersectionType::NotImplemented;

		}

		IntersectionType::Type FrustumTriangle(const glm::vec4 planes[6],
			const glm::vec3& triP0, const glm::vec3& triP1, const glm::vec3& triP2,
			float epsilon)
		{
			unsigned int inside = 0;

			for (unsigned int i = 0; i < 6; ++i)
			{
				if (PlaneTriangle(planes[i], triP0, triP1, triP2, epsilon) == IntersectionType::Inside)
					inside++;
				// in case triangle is position at outside of one plane, it is outside
				else if (PlaneTriangle(planes[i], triP0, triP1, triP2, epsilon) == IntersectionType::Outside)
					return IntersectionType::Outside;
			}
			// in case all of points are inside of planes 
			if (inside == 6)
				return IntersectionType::Inside;
			// in other cases
			else
				return IntersectionType::Overlaps;
		}

		IntersectionType::Type FrustumSphere(const glm::vec4 planes[6],
			const glm::vec3& sphereCenter, float sphereRadius, size_t& lastAxis)
		{
			/******Student:Assignment1******/
			unsigned int inside = 0;
			// same as triangle one
			for (unsigned int i = 0; i < 6; ++i)
			{
				if (PlaneSphere(planes[i], sphereCenter, sphereRadius) == IntersectionType::Inside)
					inside++;
				// in case sphere is position at outside of one plane, it is outside
				else if (PlaneSphere(planes[i], sphereCenter, sphereRadius) == IntersectionType::Outside)
					return IntersectionType::Outside;
			}
			// in case all of points are inside of planes 
			if (inside == 6)
				return IntersectionType::Inside;
			// in other cases
			else
				return IntersectionType::Overlaps;
		}

		IntersectionType::Type FrustumAabb(const glm::vec4 planes[6],
			const glm::vec3& aabbMin, const glm::vec3& aabbMax, size_t& lastAxis)
		{
			//http://www.gamedev.net/topic/512123-fast--and-correct-frustum---aabb-intersection/
			glm::vec3 vmin, vmax;
			unsigned int inside = 0;

			for (int i = 0; i < 6; ++i) {
				// X axis 
				if (planes[i].x > 0) {
					vmin.x = aabbMin.x;
					vmax.x = aabbMax.x;
				}
				else {
					vmin.x = aabbMax.x;
					vmax.x = aabbMin.x;
				}
				// Y axis 
				if (planes[i].y > 0) {
					vmin.y = aabbMin.y;
					vmax.y = aabbMax.y;
				}
				else {
					vmin.y = aabbMax.y;
					vmax.y = aabbMin.y;
				}
				// Z axis 
				if (planes[i].z > 0) {
					vmin.z = aabbMin.z;
					vmax.z = aabbMax.z;
				}
				else {
					vmin.z = aabbMax.z;
					vmax.z = aabbMin.z;
				}
				if (glm::dot(glm::vec3(planes[i].x, planes[i].y, planes[i].z), vmin) + planes[i].w > 0)
					return IntersectionType::Outside;
				else if (glm::dot(glm::vec3(planes[i].x, planes[i].y, planes[i].z), vmax) + planes[i].w >= 0)
					continue;
				else
					inside++;
			}
			if (inside == 6)
				return IntersectionType::Inside;
			// in other cases
			else
				return IntersectionType::Overlaps;
		}

		bool SphereSphere(const glm::vec3& sphereCenter0, float sphereRadius0,
			const glm::vec3& sphereCenter1, float sphereRadius1)
		{
			if (glm::distance(sphereCenter0, sphereCenter1) <= sphereRadius0 + sphereRadius1)
				return true;
			else return false;

		}

		bool AabbAabb(const glm::vec3& aabbMin0, const glm::vec3& aabbMax0,
			const glm::vec3& aabbMin1, const glm::vec3& aabbMax1)
		{
			/******Student:Assignment1******/
			glm::vec3 aabbc0((aabbMin0 + aabbMax0) / 2.f);
			glm::vec3 aabbc1((aabbMin1 + aabbMax1) / 2.f);
			glm::vec3 aabbhalf0(aabbMax0 - aabbc0);
			glm::vec3 aabbhalf1(aabbMax1 - aabbc1);

			if (abs(aabbc0[0] - aabbc1[0]) > (aabbhalf0[0] + aabbhalf1[0])) return false;
			if (abs(aabbc0[1] - aabbc1[1]) > (aabbhalf0[1] + aabbhalf1[1])) return false;
			if (abs(aabbc0[2] - aabbc1[2]) > (aabbhalf0[2] + aabbhalf1[2])) return false;

			// We have an overlap
			return true;
		}
	}	
}
