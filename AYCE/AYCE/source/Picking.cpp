/*****************************************************************************/
/*!
\file   Picking.cpp
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
// Precompiled Header
#include "Picking.h"

namespace BH
{
	void GetPickingInfo(const glm::mat4 & world,
		const glm::mat4 & view,
		const glm::mat4 & perspective,
		const glm::vec2 & mousepos,
		const glm::vec2 & screenDimension,
		glm::vec3 & clickNear, glm::vec3 & clickFar)
	{
		glm::vec2 mousePos = glm::vec2::Clamp(mousepos, glm::vec2::ZERO, screenDimension);

		glm::vec3 winCoord(mousePos.x, mousePos.y, 0.0f);

		clickNear = glm::vec3::Unproject(winCoord, 0, 0, screenDimension.x, screenDimension.y, 0.0f, 1.0f, perspective, view, world);

		winCoord.z = 1.0f;
		clickFar = glm::vec3::Unproject(winCoord, 0, 0, screenDimension.x, screenDimension.y, 0.0f, 1.0f, perspective, view, world);
	}

	float GetPickingRadius(const glm::vec3 & scale,
		const glm::mat4 & view)
	{
		float radius = 0.5f * std::max(scale.x, std::max(scale.y, scale.z));

		float x_length = glm::vec3(view.m[0][0], view.m[1][0], view.m[2][0]).Length();
		float y_length = glm::vec3(view.m[0][1], view.m[1][1], view.m[2][1]).Length();
		float z_length = glm::vec3(view.m[0][2], view.m[1][2], view.m[2][2]).Length();

		float l = std::max(x_length, std::max(y_length, z_length));
		radius *= l;

		return radius;
	}


	float RayOBBIntersection(const glm::vec3& ray, const glm::vec3& clickpoint,
		const glm::vec3& pos, const glm::vec3& scale,
		const glm::vec3 axis[3])
	{
		float tMin = 0.0f;
		float tMax = FLT_MAX;

		glm::vec3 delta = pos - clickpoint;

		glm::vec3 aabb_min = -0.5f * scale;
		glm::vec3 aabb_max = 0.5f * scale;

		// Test intersection with the 2 planes perpendicular to the OBB's axis
		for (s32 i = 0; i < 3; ++i)
		{
			float e = axis[i].Dot(delta);
			float f = ray.Dot(axis[i]);

			if (fabs(f) > FLT_EPSILON)
			{
				// Standard case
				float t1 = (e + aabb_min[i]) / f; // Intersection with the "near" plane
				float t2 = (e + aabb_max[i]) / f; // Intersection with the "far" plane
												// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

												// We want t1 to represent the nearest intersection, 
												// so if it's not the case, invert t1 and t2
				if (t1 > t2)
					std::swap(t1, t2);

				// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
				tMax = std::min(tMax, t2);

				// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
				tMin = std::max(tMin, t1);

				// And here's the trick :
				// If "far" is closer than "near", then there is NO intersection.
				// See the images in the tutorials for the visual explanation.
				if (tMax < tMin)
					return -1.0f;

			}
			else
			{
				// Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
				if (-e + aabb_min[i] > 0.0f || -e + aabb_max[i] < 0.0f)
					return -1.0f;
			}
		}

		return tMin;
	}

	float RayPlaneIntersection(const glm::vec3 & ray, const glm::vec3 & clickpoint,
		const glm::vec3 & pos,
		const glm::vec3 & normal)
	{
		//glm::vec3 d = pos - clickpoint;

		// trival rejection, parallel
		float norm_dot_ray = normal.Dot(ray);

		if (fabs(norm_dot_ray) <= FLT_EPSILON)
			return 0.0f;

		float t = (normal.Dot(pos) - normal.Dot(clickpoint)) / norm_dot_ray;

		return t;
	}

	float RayPlaneEdgeIntersection(const glm::vec3 & ray, const glm::vec3 & clickpoint,
		const glm::vec3 & pos, float radius,
		const glm::vec3 & normal)
	{
		const float PICKING_THRESHOLD = 0.5f;

		float t = RayPlaneIntersection(ray, clickpoint, pos, normal);

		if (t <= 0.0f)
			return -1.0f;

		// points of intersection
		glm::vec3 i = clickpoint + t * ray;

		float radius_square = radius * radius;

		// If click near edge
		if (fabs(glm::vec3::DistanceSquared(i, pos) - radius_square) <= PICKING_THRESHOLD * radius_square)
			return t;
		else
			return -1.0f;
	}

	float RaySphereIntersection(const glm::vec3 & viewpoint, const glm::vec3 & clickpoint,
		const glm::vec3 & center, float radius)
	{
		glm::vec3 q = clickpoint - viewpoint;

		glm::vec3 c_e = center - viewpoint;
		float c_e_sq_length = c_e.Dot(c_e);

		glm::vec3 c_p = center - clickpoint;

		float sq_radius = radius * radius;

		// trivial accpetance (Inside sphere)
		if (c_p.Dot(c_p) < sq_radius && c_e_sq_length < sq_radius)
			return 0.0f;

		float m = c_e.Dot(q);

		// trivial rejection (Going opposite direction)
		if (m < 0.0f)
			return -1.0f;

		float q_length = q.Length();
		m /= q_length;

		float sq_n = c_e_sq_length - m * m;

		// trivial rejection (Going toward but miss the sphere)
		if (sq_n > sq_radius)
			return -1.0f;

		float s = sqrt(sq_radius - sq_n);

		return m - s;
	}

	Hash Pick(const std::unordered_map< Hash, PickableInstance > & pickables,
		const glm::mat4 & world,
		const glm::mat4 & view,
		const glm::mat4 & perspective,
		const glm::vec2 & mousepos,
		const glm::vec2 & screenDimension)
	{
		glm::vec3 clickNear, clickFar;
		GetPickingInfo(world, view, perspective, mousepos, screenDimension, clickNear, clickFar);
		glm::vec3 ray = clickFar - clickNear;
		ray.Normalize();

		float minTime = FLT_MAX;
		Hash minChoice = "";

		for (const auto & i : pickables)
		{
			glm::mat4 rot = glm::mat4::CreateFromYawPitchRoll(i.second.mRotation.y, i.second.mRotation.x, i.second.mRotation.z);
			glm::mat4 trans = glm::mat4::CreateTranslation(i.second.mPosition);
			glm::mat4 scale = glm::mat4::CreateScale(i.second.mScale);

			glm::vec3 axis[3] = {
				glm::vec3(rot.m[0][0], rot.m[0][1], rot.m[0][2]),
				glm::vec3(rot.m[1][0], rot.m[1][1], rot.m[1][2]),
				glm::vec3(rot.m[2][0], rot.m[2][1], rot.m[2][2])
			};

			trans = trans * scale;

			glm::vec3 n_min = trans * i.second.mAABB.mMin;
			glm::vec3 n_max = trans * i.second.mAABB.mMax;
			glm::vec3 n_scale = n_max - n_min;
			glm::vec3 n_pos = n_min + 0.5f * n_scale;

			glm::mat4 trans_o = glm::mat4::CreateTranslation(n_pos - i.second.mPosition);

			trans = glm::mat4::CreateTranslation(i.second.mPosition);
			scale = glm::mat4::CreateScale(n_scale);

			trans = trans * rot * trans_o * scale;

			//n_pos = glm::vec3( trans.m[0][3], trans.m[1][3], trans.m[2][3] );
			n_pos = glm::vec3(trans.m[3][0], trans.m[3][1], trans.m[3][2]);

			float t = RayOBBIntersection(ray, clickNear, n_pos, n_scale, axis);
			if (t >= 0.0f)
			{
				if (minChoice == "" || t < minTime)
				{
					minTime = t;
					minChoice = i.first;
				}
			}
		}

		return minChoice;
	}
}