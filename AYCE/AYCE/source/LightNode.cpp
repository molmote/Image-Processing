/*****************************************************************************/
/*!
\file   LightNode.cpp
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
#include "LightNode.h"

namespace VK
{
	const GLfloat dropRatio = 0.05f;

	LightNode::LightNode(float initialIntensity, float ir, float ig, float ib) :
		intensity(initialIntensity),
		r(ir),
		g(ig),
		b(ib)
	{
		radius = sqrtf(intensity / dropRatio);
		getTransform().setScale(radius, radius, radius);
	}


	LightNode::~LightNode()
	{
	}

	void LightNode::setMesh(Mesh* m) {
		mesh = m;
	}

	void LightNode::setRGB(float rr, float gg, float bb) {
		r = rr;
		g = gg;
		b = bb;
	}

	//void LightNode::pointLightPass(GraphicsRenderer* manager) {
	//	Camera* camera = VK::Camera::GetInstance();
	//	//ShaderProgram* shaderProgram = manager->getPointLightProgram();
	//	Shader* shaderProgram = manager->getPointLightPr();

	//	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * getTransform().getTransformation();
	//	//shaderProgram->setUniformMatrix4fv("mvp", 1, false, glm::value_ptr(mvp));
	//	//shaderProgram->setUniform1f("lightIntensity", intensity);
	//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->Program, "mvp"), 1, false, glm::value_ptr(mvp));
	//	glUniform1f(glGetUniformLocation(shaderProgram->Program, "lightIntensity"), intensity);

	//	glm::vec3& lightPos = getTransform().getTranslation();
	//	//shaderProgram->setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	//	//shaderProgram->setUniform3f("lightColor", r, g, b);
	//	glUniform3f(glGetUniformLocation(shaderProgram->Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	//	glUniform3f(glGetUniformLocation(shaderProgram->Program, "lightColor"), r, g, b);

	//	mesh->Draw(*shaderProgram);
	//}

	//void LightNode::stencilPass(GraphicsRenderer* manager) {
	//	Camera* camera = VK::Camera::GetInstance();
	//	//ShaderProgram* shaderProgram = manager->getStencilProgram();
	//	Shader* shaderProgram = manager->getStencilPr();

	//	glm::mat4 mvp = camera->GetProjectionMatrix() * camera->GetViewMatrix() * getTransform().getTransformation();


	//	//shaderProgram->setUniformMatrix4fv("mvp", 1, false, glm::value_ptr(mvp));
	//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram->Program, "mvp"), 1, false, glm::value_ptr(mvp));
	//	mesh->Draw(*shaderProgram);
	//}
}