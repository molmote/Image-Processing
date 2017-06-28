/*****************************************************************************/
/*!
\file   ModelNode.cpp
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
#include "ModelNode.h"

namespace VK
{
	ModelNode::ModelNode() :
		mesh(nullptr)
	{
	}

	ModelNode::ModelNode(Mesh* m) :
		mesh(m)
	{
	}

	ModelNode::~ModelNode()
	{
	}

	void ModelNode::setMesh(Mesh* m) {
		mesh = m;
	}

	Mesh* ModelNode::getMesh() const
	{
		return mesh;
	}

	//void ModelNode::geometryPass(GraphicsRenderer* manager) {
	//	if (mesh != nullptr) {
	//		//ShaderProgram* shaderProgram = manager->getGeometryProgram();
	//		Shader* shaderProgram = manager->getGeometryPr();
	//		//CameraNode* camera = manager->getCameraNode();
	//		Camera* camera = VK::Camera::GetInstance();
	//		camera->UpdateCamera();
	//		glm::mat4 mv = camera->GetViewMatrix() *getTransform().getTransformation();
	//		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(getTransform().getTransformation()));
	//		glm::mat4 mvp = camera->GetProjectionMatrix() * mv;

	//		/*shaderProgram->setUniformMatrix4fv("model", 1, false, glm::value_ptr(getTransform().getTransformation()));
	//		shaderProgram->setUniformMatrix4fv("mvp", 1, false, glm::value_ptr(mvp));
	//		shaderProgram->setUniformMatrix3fv("normalMatrix", 1, false, glm::value_ptr(normalMatrix));*/
	//		glUniformMatrix4fv(glGetUniformLocation(shaderProgram->Program, "model"), 1, false, glm::value_ptr(getTransform().getTransformation()));
	//		glUniformMatrix4fv(glGetUniformLocation(shaderProgram->Program, "mvp"), 1, false, glm::value_ptr(mvp));
	//		glUniformMatrix3fv(glGetUniformLocation(shaderProgram->Program, "normalMatrix"), 1, false, glm::value_ptr(normalMatrix));
	//		mesh->Draw(*shaderProgram);
	//	}
	//}
}