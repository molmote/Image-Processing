/*****************************************************************************/
/*!
\file   ShaderManager.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for ShaderManager features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include <string>
#include <unordered_map>
#include "precompiled.h"
#include "GBuffer.h"
#include "ShaderProgram.h"
#include "GraphicsRenderer.h"
namespace VK
{
	class ShaderManager
	{
	public:
		ShaderManager(void);
		~ShaderManager(void);

		static ShaderManager* GetInstance();


		bool gameCreatePixelShader(const char* fileName);
		bool gameCreateVertexShader(const char* fileName);
		bool gameInitGLSL();

	//	GBuffer* getGbuffer() const { return gbuffer; }

	private:
		std::string	LoadShaderFromFile(const char* filename) const;
	//	GBuffer* gbuffer;

		//std::unordered_map<string, ShaderProgram> shaders;

	private:
		static ShaderManager* m_pInstance;
	};



	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Program id
	extern unsigned programId;

	// Frame counter
	extern unsigned sFrameCtr;

	// Animation flag
	extern bool sAnmFlag;


	// Number of lights
	extern unsigned numberOfLights;

	// Shader loading functions
	bool gameCreateVertexShader(const char* fileName);
	bool gameCreatePixelShader(const char* fileName);

	bool gameInitGLSL();
} //namespace VK