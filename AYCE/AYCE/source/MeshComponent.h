/*****************************************************************************/
/*!
\file   MeshComponent.h
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
#pragma once
#include "Component.h"
#include "precompiled.h"
#include "model.h"
#include <unordered_map>


namespace VK
{
	class Mesh;
	class Material;

	class MeshComponent : public Component
	{
	public:
		MeshComponent();
		virtual ~MeshComponent();

		void setMesh(std::string object);
		Mesh* getMesh() const;
		Model* MeshComponent::getModel();
		void setMeshFile(std::string filename);
		std::string getMeshFile() const;

		Material* getMaterial() const;

		//glm::mat4 getModel();

		

		//static MeshComponent* GetInstance();
		
	private:
		
		Mesh* m_mesh;
		Material* m_material;
		std::string m_meshFile;
		Model* m_model;

		//static MeshComponent* m_Instance;		
	};
}
