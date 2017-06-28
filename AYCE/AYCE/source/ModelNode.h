/*****************************************************************************/
/*!
\file   ModelNode.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for ModelNode features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"
#include "GraphicsRenderer.h"
//#include "ShaderProgram.h"
//#include "CameraNode.h"
#include "Camera.h"
#include "SceneNode.h"
//#include "Mesh.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace VK
{
	/*
	enum class MODEL_FLAG : uint64_t
	{
		GEOMETRY,
		LIGHT,
		LAST_FLAG
	};
	*/

	class ModelNode : public SceneNode
	{
	public:
		ModelNode();
		ModelNode(Mesh* m);
		~ModelNode();
		//void geometryPass(GraphicsRenderer* shaderProgram);
		Mesh* getMesh() const;
		void setMesh(Mesh* m);

		glm::vec3 direction;
		glm::vec3 velocity;
		
		std::string types;
	private:
		Mesh* mesh;
	};
}
