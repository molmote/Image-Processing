/*****************************************************************************/
/*!
\file   Utilities.cpp
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
#include "Utilities.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "InputSystem.h"
#include "Application.h"
#include "Camera.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"

namespace Utilities
{
	float RandMinMax(const float Min, const float Max)
	{
		if (Min == Max)
			return Min;

		return Min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (Max - Min)));
	}

	int RandMinMax(const int Min, const int Max)
	{
		if (Min == Max)
			return Min;

		return Min + rand() / (RAND_MAX / (Max - Min));
	}

	float RayAabb(const glm::vec3& rayStart, const glm::vec3& rayDir, const glm::vec3& aabbMin, const glm::vec3& aabbMax, float& t)
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

	VK::Entity* Picking(void)
	{

		VK::Application* app = VK::Application::GetAppInstance();
		VK::InputSystem* m_pIS = app->getSystem<VK::InputSystem>();
		VK::EntitySystem* m_pES = app->getSystem<VK::EntitySystem>();

		VK::Camera* pCamera = VK::Camera::GetInstance();

		VK::Entity* pickedEntity = nullptr;

		float x = static_cast<float>((2.0f * m_pIS->GetMouseX())/ app->GetPrevWinWidthSize() - 1.0f);
		float y = static_cast<float>(1.0f - (2.0f*m_pIS->GetMouseY())/ app->GetPrevWinHeightSize());
		float z = 1.0f;

		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds.x,ray_nds.y, -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(pCamera->GetProjectionMatrix()) *ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

		glm::vec4 ray_world = (glm::inverse(pCamera->GetViewMatrix())*ray_eye);
		glm::vec3 ray_direction = glm::vec3(ray_world.x, ray_world.y, ray_world.z);
		ray_direction = glm::normalize(-ray_direction);

		VK::EntityListVector e_DynamicList = m_pES->GetDynamicEntityList();
		float tmin = FLT_MAX;
		for (auto it : e_DynamicList)
		{
			VK::TransformComponent* pTransform = it->FindComponent<VK::TransformComponent>();
			VK::RigidBodyComponent* pRigidBody = it->FindComponent<VK::RigidBodyComponent>();

			float t;
			if (RayAabb(pCamera->GetPosition(), ray_direction, pRigidBody->GetAAbbMin(), pRigidBody->GetAAbbMax(), t))
			{
				if (tmin > t)
				{
					tmin = t;
					pickedEntity = it;
				}
			}
		}


		return pickedEntity;
	}

}