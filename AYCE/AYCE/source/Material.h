/*****************************************************************************/
/*!
\file   Material.h
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Material features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

namespace VK
{
	class Material
	{
	public:
		Material();
		~Material();

		void BindProgram();
		void BindUniforms();
		void Unbind();

		void SetUniform();
	private:
		int m_programID;
	};
}