
/*****************************************************************************/
/*!
\file   precompiled.h
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Picking features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once

//#define DIRECTINPUT_HEADER_VERSION  0x0800
//#ifndef DIRECTINPUT_VERSION
//#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION
//#pragma message(__FILE__ ": DIRECTINPUT_VERSION undefined. Defaulting to version 0x0800")
//#endif

#define DIRECTINPUT_VERSION 0x0800 
#define _USE_MATH_DEFINES 
#define GLFW_INCLUDE_GL_3

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <memory.h>
#include <string.h>
#include <conio.h>
#include <assert.h>
#include <float.h>
#include <direct.h>
#include <dinput.h>

#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>


#include <map>
#include <typeinfo>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <memory>
#include <fstream>
#include <deque>
#include <iterator>
#include <array>
#include <tuple>
#include <chrono>
#include <ctime>
#include <iomanip>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

//#include <vld.h> 
#include "flagset.h"

#include "log.h"

#include "ResouceManager.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define CLIP_NEAR 0.1f
#define CLIP_FAR  5000.f
#define SAFE_DELETE(x) {if(x != nullptr){delete x; x = nullptr;}}
