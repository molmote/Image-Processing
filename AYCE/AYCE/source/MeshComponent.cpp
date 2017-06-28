/*****************************************************************************/
/*!
\file   MeshComponent.cpp
\author Yeongki Baek, Dokyoon Kim
\par    email: yeongki.baek\@digipen.edu, dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for MeshComponent features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "MeshComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "Mesh.h"
#include "Material.h"
#include <filesystem>


namespace VK
{
	MeshComponent::MeshComponent(): m_material(nullptr), m_mesh(nullptr), m_model(nullptr)
	{
	}

	MeshComponent::~MeshComponent()
	{
//		SAFE_DELETE(m_mesh);
//		SAFE_DELETE(m_material);
//		SAFE_DELETE(m_model);
	}
	void MeshComponent::setMesh(std::string object)
	{
		m_model = ResourceManager::GetInstance()->setModel(object);
		m_meshFile = object;
	}
	Mesh* MeshComponent::getMesh() const
	{
		return m_mesh;
	}

	Model* MeshComponent::getModel()
	{
		return m_model;
	}

	void MeshComponent::setMeshFile(std::string filename)
	{
	}

	std::string MeshComponent::getMeshFile() const
	{
		return m_meshFile;
	}
	
	Material* MeshComponent::getMaterial() const
	{
		return m_material;
	}

	/*glm::mat4 MeshComponent::getModel()
	{
		TransformComponent* transform = dynamic_cast<TransformComponent*>(GetOwner()->FindComponent<TransformComponent>());
		glm::mat4 m;
		return m;
	}*/
}
