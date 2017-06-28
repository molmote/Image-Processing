/*****************************************************************************/
/*!
\file   GBuffer.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for GBuffer features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

#include "precompiled.h"
#include "glew\glew.h"
#include <iostream>

#ifndef NULL
#define NULL 0
#endif 

namespace VK
{

	class GBuffer
	{
	public:
		enum GBUFFER_TEXTURE_TYPE {
			GBUFFER_TEXTURE_TYPE_POSITION,
			GBUFFER_TEXTURE_TYPE_DIFFUSE,
			GBUFFER_TEXTURE_TYPE_NORMAL,
			GBUFFER_TEXTURE_TYPE_TEXCOORD,
			GBUFFER_NUM_TEXTURES
		};

		GBuffer();
		~GBuffer();

		void init(int width, int height);
		void startFrame() const;
		void bindForGeomPass() const;
		void bindForStencilPass();
		void bindForLightPass();
		void bindForFinalPass();
		void bindForBlitting();
		void setReadBuffer(GBUFFER_TEXTURE_TYPE textureType);

		void reset(int width, int height);

		static GBuffer* GetInstance();

		
	private:
		GLuint fbo;
		GLuint textures[GBUFFER_NUM_TEXTURES];
		GLuint depthTexture;
		GLuint finalTexture;

		static GBuffer* m_pInstance;
	};


} // namespace VK;