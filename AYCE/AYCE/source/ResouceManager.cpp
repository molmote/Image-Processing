/*****************************************************************************/
/*!
\file   ResourceManager.cpp
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM350
\date   03/14/2017
\brief
This is the interface file for Camera features
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "ResouceManager.h"
#include "Model.h"
namespace VK
{
	ResourceManager* ResourceManager::m_Instance = nullptr;

	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
		//m_meshses.erase(m_meshses.begin(), m_meshses.end());

		for (auto it = m_meshses.begin(); it != m_meshses.end();) {
			delete it->second;
			it = m_meshses.erase(it);
		}
		//m_meshses.clear();
		
		m_Instance = nullptr;

	}

	Model* ResourceManager::setModel(std::string object)
	{
		if (m_meshses.find(object) == m_meshses.end())
		{
			m_meshses[object] = new Model((object).c_str());
			m_modelList.push_back(object);
		}
		return m_meshses[object];
	}

	Model* ResourceManager::getModel(std::string object)
	{
		if (m_meshses.find(object) != m_meshses.end())
			return m_meshses[object];

		return nullptr;
	}

	std::vector<std::string> ResourceManager::GetModelList()
	{
		return m_modelList;
	}

	ResourceManager* ResourceManager::GetInstance()
	{
		if (!m_Instance)
			m_Instance = new ResourceManager();
		return m_Instance;

	}

	
} //namespace VK