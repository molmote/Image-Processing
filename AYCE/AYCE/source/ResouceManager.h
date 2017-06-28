/*****************************************************************************/
/*!
\file   ResourceManager.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM350
\date   03/14/2017
\brief
This is the interface file for Camera features
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include <unordered_map>
#include <string>

namespace VK
{
	class Model;

	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();

		static ResourceManager* GetInstance();

		Model* setModel(std::string object);
		Model* getModel(std::string object);

		std::vector<std::string> GetModelList();

		std::vector<std::string> m_modelList;

	private:
		std::unordered_map<std::string, Model*> m_meshses;

		static ResourceManager* m_Instance;
	};

} //namespace VK