/*****************************************************************************/
/*!
\file   LevelEditor.h
\author Dokyoom Kim
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2017
\brief
Header file of Level Editor State
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

#include "State.h"
#include "Application.h"
#include "precompiled.h"

namespace VK
{
	class AudioSystem;
	class InputSystem;
	class EntitySystem;
	class PhysicsSystem;
	class ImguiManager;
	class StateSystem;
	class Entity;
}

class LevelEditor : public VK::State
{
public:
	LevelEditor();
	~LevelEditor();

	void Enter() override;
	void Update(float dt) override;
	void Exit() override;
private:
	VK::Application* m_app = VK::Application::GetAppInstance();
	VK::AudioSystem* p_audioSystem;
	VK::InputSystem* p_inputSystem;
	VK::EntitySystem* p_entitySystem;
	VK::ImguiManager* p_imguiSystem;
	VK::StateSystem* p_stateSystem;

	std::string levelfile;
	bool m_soundEditor_window;
	bool m_createEntity_window;
	bool entityProperty_window;
	bool m_deleteEntity_window;
	bool audioSave;
	bool levelSave;
	//int m_entityType;

	void MainEditor();
	void SoundEditorWindow();
	void CreateEntityWindow();
	void EntityPropertyWindow(VK::Entity* pEntity);
	void ComponentProperty(VK::Entity* pEntity);
	void SaveAudioWindow();
	void SaveLevelWindow();
	void DeleteEntityWindow(VK::Entity* pEntity);
	//std::string openfilename(LPTSTR str);

	std::vector<char const *> m_levelname;
	std::vector<char const *> m_audioname;
	int m_levelnumber;
	int m_audionumber[5];
	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rot;
	glm::vec2 holeSize;

	VK::Entity* pChoosenEntity;
	double xstart;
	double ystart;
	bool check;
};

