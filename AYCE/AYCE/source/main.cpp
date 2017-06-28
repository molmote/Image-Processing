/*****************************************************************************/
/*!
\file   main.cpp
\author Yeongki Baek, Dokyoon Kim
\par    email: yeongki.baek\@digipen.edu, dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for main features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "precompiled.h"
#include <stdlib.h>

#include "Application.h"
#include "System.h"
#include "AudioSystem.h"
#include "EntitySystem.h"
#include "InputSystem.h"
#include "StateSystem.h"
#include "GraphicsRenderer.h"
#include "ImguiManager.h"
#include "Physics.h"
#include "GameLogic.h"
#include "Particle.h"
#include <time.h>


int main(int argc, char** argv)
{
#if defined(WIN32) || defined(WIN64)
#ifdef _DEBUG
#include <crtdbg.h>


//#define _CRTDBG_MAP_ALLOC
	_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif
#endif

	VK::Application* game = VK::Application::GetAppInstance();// atoi(argv[0]), atoi(argv[1]));

	srand((unsigned)time(NULL));

	using namespace VK;
	game->addSystem(new AudioSystem);
	game->addSystem(new EntitySystem);
	game->addSystem(new GraphicsRenderer);
	game->addSystem(new PhysicsSystem);
	game->addSystem(new ImguiManager);
	game->addSystem(new Particle);
	game->addSystem(new GameLogic);	
	game->addSystem(new StateSystem);
	game->addSystem(new InputSystem);
	


	game->Initialize();
	game->Run();
	game->Shutdown();

	delete game;


	return 0;
}