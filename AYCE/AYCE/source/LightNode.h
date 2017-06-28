/*****************************************************************************/
/*!
\file   LightNode.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for LightNode features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

#include "precompiled.h"
//#include <glm/mat4x4.hpp>
//#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "SceneNode.h"
#include "Mesh.h"
#include "Camera.h"
#include "GraphicsRenderer.h"

namespace VK
{
	class LightNode :
		public SceneNode
	{
	public:
		LightNode(float initialIntensity, float ir, float ig, float ib);
		~LightNode();
//		virtual void pointLightPass(GraphicsRenderer* manager);
//		void stencilPass(GraphicsRenderer* manager);
		void setMesh(Mesh* m);
		void setRGB(float rr, float gg, float bb);
		glm::vec3 direction;
	private:
		Mesh* mesh;
		GLfloat intensity;
		float r;
		float g;
		float b;
		float radius;
	};
}
