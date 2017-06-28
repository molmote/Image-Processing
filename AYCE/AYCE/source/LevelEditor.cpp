/*****************************************************************************/
/*!
\file   LevelEditor.cpp
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2017
\brief
This is the interface file for LevelEditor features
Copyright 2017, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "LevelEditor.h"
#include "InputSystem.h"
#include "Factory.h"
/*****************************************************************************/
/*!
\file   LevelEditor.cpp
\author Dokyoom Kim
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
Level Editor State
*/
/*****************************************************************************/
#include "AudioSystem.h"
#include "Camera.h"
#include "ImguiManager.h"
#include "StateSystem.h"
#include "EntitySystem.h"
#include "imgui.h"
#include <iostream>
//#include <string>
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "CharacterMove.h"
//#include "Factory.h"
//#include "Camera.h"
#include "MeshComponent.h"
#include "Utilities.h"

static std::vector<std::string> get_all_files_names_within_folder(std::string folder, std::string fileformat);
static std::vector<const char*> constcharswitch(std::vector<std::string> v)
{
	std::vector<char const *> temp;
	
	for (unsigned i = 0; i< v.size(); ++i)
	{
		char* writable = new char[v[i].size() + 1];
		std::copy(v[i].begin(), v[i].end(), writable);
		writable[v[i].size()] = '\0';
		//temp[i] = v[i].c_str();
		temp.push_back(const_cast<const char*>(writable));
	}

	return temp;
}

static void CameraMove(double&  startx, double& starty, bool& check);

LevelEditor::LevelEditor()
{
	p_inputSystem = nullptr;
	p_entitySystem = nullptr;
	p_audioSystem = nullptr;
	p_imguiSystem = nullptr;
	p_stateSystem = nullptr;
	m_soundEditor_window = false;
	m_createEntity_window = false;
	entityProperty_window = false;
	audioSave = false;
	levelSave = false;
	m_deleteEntity_window = false;
	pChoosenEntity = nullptr;
	//m_entityType = 0;
	xstart = 0;
	ystart = 0;
	check = false;

	for (int i = 0; i < 5; i++)
	{
		m_audionumber[i] = 0;
	}
	m_levelnumber = 0;
}
LevelEditor::~LevelEditor()
{

}

void LevelEditor::Enter()
{
	p_inputSystem = m_app->getSystem<VK::InputSystem>();
	p_entitySystem = m_app->getSystem<VK::EntitySystem>();
	p_audioSystem = m_app->getSystem<VK::AudioSystem>();
	p_imguiSystem = m_app->getSystem<VK::ImguiManager>();
	p_stateSystem = m_app->getSystem<VK::StateSystem>();

	std::vector<std::string> audiofiles = get_all_files_names_within_folder("\\Audio", "/*.*");
	m_audioname = constcharswitch(audiofiles);
	///
	VK::AudioDataList audioList = p_audioSystem->GetAudioDataList();
	for(auto it :audioList)
	{
		for (unsigned i = 0; i < audiofiles.size(); i++)
		{
			std::string audioname = "Audio\\" + audiofiles[i];
			if (it.second->filename == audioname)
				m_audionumber[it.first] = i;
		}
	}
	VK::Camera::GetInstance()->SetPosition(glm::vec3(0, 600, 600));

	
}
void LevelEditor::Update(float dt)
{
	p_imguiSystem->NewFrame();
	//camera
	CameraMove(xstart, ystart, check);

	//ImVec4 clear_color = ImColor(114, 144, 154);
	int w, h;
	glfwGetWindowSize(m_app->GetWindow(), &w, &h);
	glViewport(0, 0, w, h);

	
	MainEditor();
	

	ImGui::Render();
	if (p_inputSystem->IsPressed(VK::KEY_ESCAPE))
	{
		p_stateSystem->SetNextState(VK::STATE_MENU);
	}

}
void LevelEditor::Exit()
{
	for (unsigned i = 0; i < m_levelname.size(); i++)
	{
		delete[] m_levelname[i];
	}
	for (unsigned i = 0; i < m_audioname.size(); i++)
	{
		delete[] m_audioname[i];
	}
}

void LevelEditor::MainEditor()
{


	ImGui::Begin("Level Editor");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(300, 400));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


	std::vector<std::string> levelfiles = get_all_files_names_within_folder("\\Object", "/Level*.*");
	m_levelname = constcharswitch(levelfiles);

	ImGui::Combo("", &m_levelnumber, m_levelname.data(), m_levelname.size());
	ImGui::SameLine();
	if (ImGui::Button("Select Level"))
	{
		p_entitySystem->ClearEntity();
		Factory::CreateLevel(m_levelname[m_levelnumber]);
	}
	ImGui::Separator();
	ImGui::Text("Entities");
	if (ImGui::Button("Create Entity"))
	{
		m_createEntity_window = m_soundEditor_window ? false : true;
	}

	if (m_createEntity_window)
	{
		CreateEntityWindow();
	}
	//////////////////////////////////////////////////////////////

	
	//if (p_inputSystem->IsTriggered(VK::KEY_MOUSE_LEFT))
	//{

	//	pChoosenEntity = Utilities::Picking();

	//	if (pChoosenEntity)
	//	{
	//		entityProperty_window = true;
	//		EntityPropertyWindow(pChoosenEntity);

	//	}
	//}
	VK::EntityListVector eList = p_entitySystem->GetDynamicEntityList();

	for (auto it : eList)
	{
		ImGui::PushID(it->GetName().c_str());
		if (ImGui::CollapsingHeader(it->GetName().c_str()))
		{
			if (ImGui::Button("Property"))
			{
				//pChoosenEntity = pChoosenEntity == it ? nullptr : it;
				entityProperty_window = entityProperty_window == false ? true : false;
			}
			if(entityProperty_window)
				EntityPropertyWindow(it);

			if (ImGui::Button("Delete"))
			{
				//pChoosenEntity = pChoosenEntity == it ? nullptr : it;
				m_deleteEntity_window = m_deleteEntity_window == false ? true : false;

			}
			if (m_deleteEntity_window)
				DeleteEntityWindow(it);
		}
		//if (pChoosenEntity && m_deleteEntity_window)
		//	DeleteEntityWindow(pChoosenEntity);

		//if(pChoosenEntity && entityProperty_window)
		//	EntityPropertyWindow(pChoosenEntity);

		ImGui::Separator();
		ImGui::PopID();
	}
	//////////////////////////////////////////////////////////////

	ImGui::Separator();
	ImGui::Text("Sounds");
	if (ImGui::Button("Edit Sounds"))
	{
		m_soundEditor_window = m_soundEditor_window ? false : true;
	}
	if (m_soundEditor_window)
	{
		SoundEditorWindow();
	}

	if (ImGui::Button("SAVE LEVEL"))
	{
		levelSave = true;
	}
	if (levelSave)
		SaveLevelWindow();
	ImGui::End();
}
void LevelEditor::SoundEditorWindow()
{
	ImGui::SetNextWindowSize(ImVec2(400, 500));
	ImGui::Begin("Sound Property", &m_soundEditor_window);
	ImGui::SetWindowPos(ImVec2(0, 300));

	VK::AudioDataList audioList = p_audioSystem->GetAudioDataList();

	std::string enumstring;
	for (auto &it : audioList)
	{
		enumstring = p_audioSystem->TranslateAudioKey(it.first);
		ImGui::PushID(enumstring.c_str());
		if (ImGui::CollapsingHeader(enumstring.c_str()))
		{
			std::vector<std::string> audiofiles = get_all_files_names_within_folder("\\Audio","/*.*");
			m_audioname = constcharswitch(audiofiles);

		
			
			ImGui::Combo("", &m_audionumber[it.first], m_audioname.data(), m_audioname.size());
			
			ImGui::SameLine();
			if (ImGui::Button("Select Sound"))
			{
				std::string fullfile = m_audioname[m_audionumber[it.first]];
				it.second->filename = "Audio\\" + fullfile;
			}
			ImGui::SliderFloat("Volume", &it.second->volume, 0, 1,"%.1f");

		}
		ImGui::Separator();
		ImGui::PopID();
	}
	ImGui::Text("                                      ");
	ImGui::SameLine();
	if (ImGui::Button("Save Audio Data"))
	{
		audioSave = true;
	}
	if(audioSave)
		SaveAudioWindow();

	ImGui::End();

}
void LevelEditor::DeleteEntityWindow(VK::Entity* pEntity)
{


	int width, height;
	glfwGetWindowSize(m_app->GetWindow(), &width, &height);
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("Do You Want Delete Entity?", &m_deleteEntity_window);
	ImGui::SetWindowPos(ImVec2(static_cast<float>(width) / 2, static_cast<float>(height) / 2));

	ImGui::Text("       ");
	ImGui::Text("   ");
	ImGui::SameLine();

	if (ImGui::Button("DELETE"))
	{
		p_entitySystem->DeleteEntity(pEntity);
		m_deleteEntity_window = false;
	}
	ImGui::SameLine();
	ImGui::Text("     ");
	ImGui::SameLine();
	if (ImGui::Button(" NO "))
	{
		m_deleteEntity_window = false;
	}

	ImGui::End();
}

void LevelEditor::CreateEntityWindow()
{
	ImGui::SetNextWindowSize(ImVec2(300, 400));
	ImGui::Begin("Create New Entity", &m_createEntity_window);
	ImGui::SetWindowPos(ImVec2(400, 0));

	std::vector<char const *> newEntityType = { "None", "Map", "Hole" };
	static int m_entityType = 0;
	ImGui::Combo("Entity Type", &m_entityType, newEntityType.data(), newEntityType.size());
	bool save = false;
	switch (m_entityType)
	{
		//none
		case 0:
			break;
		//Map
		case 1:
			ImGui::Text("Set Values");
			ImGui::Separator();

			ImGui::InputFloat3("Position", reinterpret_cast<float*>(&pos), 1);
			ImGui::InputFloat3("Scale", reinterpret_cast<float*>(&scale), 1);
			ImGui::InputFloat3("Rotation", reinterpret_cast<float*>(&rot), 1);

			if (ImGui::Button("Create"))
			{
				save = true;
			}

			if (save)
			{
				Factory::CreateMapEntity(pos, scale, rot);
				//save = false;
				m_createEntity_window = false;
			}
			break;
		case 2:
			ImGui::Text("Set Values");
			ImGui::Separator();

			ImGui::InputFloat3("Position", reinterpret_cast<float*>(&pos), 1);
			ImGui::InputFloat2("Hole Size", reinterpret_cast<float*>(&holeSize), 1);

			if (ImGui::Button("Create"))
			{
				save = true;
			}

			if (save)
			{
				Factory::CreateHoleEntity(pos, holeSize);
				//save = false;
				m_createEntity_window = false;
			}
			break;
	default: break;
	}
	ImGui::End();
}

static std::vector<std::string> get_all_files_names_within_folder(std::string folder,std::string fileformat)
{
	std::vector<std::string> names;
	std::string search_path = folder + fileformat;
	WIN32_FIND_DATA fd;

	TCHAR _currDirectoryPath[256];
	GetCurrentDirectory(256, _currDirectoryPath);
	static TCHAR InitialDir[256];

	strcpy_s(InitialDir, sizeof(InitialDir), _currDirectoryPath);
	strcat_s(InitialDir, sizeof(InitialDir), search_path.c_str());



	HANDLE hFind = ::FindFirstFile(InitialDir, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

void LevelEditor::EntityPropertyWindow(VK::Entity* pEntity)
{
	int width, height;
	glfwGetWindowSize(m_app->GetWindow(), &width, &height);
	ImGui::SetNextWindowSize(ImVec2(400, 600));
	ImGui::Begin("Entity Property", &m_soundEditor_window);
	ImGui::SetWindowPos(ImVec2(static_cast<float>(width) - 400, 0));

	ImGui::Text(pEntity->GetName().c_str());
	ImGui::Separator();
	ImGui::Text("Components");

	ComponentProperty(pEntity);

	ImGui::End();
	
}
void LevelEditor::ComponentProperty(VK::Entity* pEntity)
{
	VK::TransformComponent* transform = pEntity->FindComponent<VK::TransformComponent>();
	//VK::RigidBodyComponent* rigidbody = pEntity->FindComponent<VK::RigidBodyComponent>();
	VK::CharacterMove* charactermove = pEntity->FindComponent<VK::CharacterMove>();
	VK::MeshComponent* mesh = pEntity->FindComponent <VK::MeshComponent>();

	if (transform)
	{
		ImGui::PushID("Transform Component");

		if (ImGui::CollapsingHeader("Transform Component"))
		{
			ImGui::InputFloat3("Position", reinterpret_cast<float*>(&transform->getTranslation()), 1);
			ImGui::InputFloat3("Scale", reinterpret_cast<float*>(&transform->getScale()), 1);
			ImGui::InputFloat3("Rotation", reinterpret_cast<float*>(&transform->getRotation()), 1);
		}
		ImGui::PopID();

	}
	if (mesh)
	{
		//ImGui::PushID("RigidBody Component");
		ImGui::Text(mesh->getMeshFile().c_str());

		//if (ImGui::CollapsingHeader("RigidBody Component"))
		//{

		//}
		//ImGui::PopID();

	}
	if (charactermove)
	{
		ImGui::PushID("CharacterMove Component");

		if (ImGui::CollapsingHeader("CharacterMove Component"))
		{
			ImGui::InputFloat("Character Speed", static_cast<float*>(&charactermove->GetSpeed()), 1);
		}
		ImGui::PopID();

	}
}

void LevelEditor::SaveAudioWindow()
{

	int width, height;
	glfwGetWindowSize(m_app->GetWindow(), &width, &height);
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("Do You Want Save Audio Data?", &audioSave);
	ImGui::SetWindowPos(ImVec2(static_cast<float>(width)/2, static_cast<float>(height)/2));

	ImGui::Text("       ");
	ImGui::Text("   ");
	ImGui::SameLine();

	if (ImGui::Button("SAVE"))
	{

			p_audioSystem->Release_Sound_All();
			Factory::SaveAudioData("AudioData.Json");
			p_audioSystem->Create_Sound_All();
			audioSave = false;

	}
	ImGui::SameLine();
	ImGui::Text("     ");
	ImGui::SameLine();
	if (ImGui::Button(" NO "))
	{
		audioSave = false;
	}
	
	ImGui::End();
}

void LevelEditor::SaveLevelWindow()
{
	int width, height;
	glfwGetWindowSize(m_app->GetWindow(), &width, &height);
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("Do You Want Save Level Data?", &levelSave);
	ImGui::SetWindowPos(ImVec2(static_cast<float>(width) / 2, static_cast<float>(height) / 2));

	ImGui::Text("       ");
	ImGui::Text("   ");
	ImGui::SameLine();

	if (ImGui::Button("SAVE"))
	{

		Factory::LevelSave(m_levelname[m_levelnumber]);
		levelSave = false;

	}
	ImGui::SameLine();
	ImGui::Text("     ");
	ImGui::SameLine();
	if (ImGui::Button(" NO "))
	{
		levelSave = false;
	}

	ImGui::End();
}

static void CameraMove(double&  startx, double& starty, bool& check)
{
	VK::Application* app = VK::Application::GetAppInstance();
	VK::InputSystem* p_inputSystem = app->getSystem<VK::InputSystem>();



	if (p_inputSystem->IsPressed(VK::KEY_MOUSE_LEFT) && !check)
	{
		startx = p_inputSystem->GetMouseX();
		starty = p_inputSystem->GetMouseY();
		check = true;

	}

	else if (/*p_inputSystem->IsPressed(VK::KEY_MOUSE_LEFT) && */check)
	{

		GLfloat xoffset = static_cast<float>(startx) - static_cast<float>(p_inputSystem->GetMouseX());
		GLfloat yoffset = static_cast<float>(p_inputSystem->GetMouseY()) - static_cast<float>(starty);

		startx = p_inputSystem->GetMouseX();
		starty = p_inputSystem->GetMouseY();

		std::cout << "xoffset : " << xoffset << std::endl;
		std::cout << "yoffset : " << yoffset << std::endl;
		system("cls");

		VK::Camera::GetInstance()->ProcessMouseMovement(xoffset, yoffset);
		check = false;
	}
}