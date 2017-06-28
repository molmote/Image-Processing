/*****************************************************************************/
/*!
\file   InputSystem.h
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for InputSystem features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"
#include "System.h"
//#include <GLFW/glfw3.h>


namespace VK
{
	enum KeyCode {
		KEY_INVALID,
		KEY_BACKSPACE,
		KEY_DELETE,
		KEY_TAB,
		KEY_RETURN,
		KEY_SHIFT,
		KEY_CONTROL,
		KEY_CAPSLOCK,
		KEY_ESCAPE,
		KEY_SPACE,
		KEY_ACCENT,
		KEY_ARROW_UP,
		KEY_ARROW_DOWN,
		KEY_ARROW_LEFT,
		KEY_ARROW_RIGHT,
		KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K,
		KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V,
		KEY_W, KEY_X, KEY_Y, KEY_Z,
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
		KEY_ENTER,
		KEY_LEFT_SHIFT,
		KEY_RIGHT_SHIFT,
		KEY_LEFT_CONTROL,
		KEY_RIGHT_CONTROL,
		KEY_MOUSE_LEFT, KEY_MOUSE_RIGHT,
		KEY_MOUSE_MIDDLE,
		KEY_MOUSE_MIDDLE_UP,
		KEY_MOUSE_MIDDLE_DOWN, KEY_ALT,
		KEY_ADD, KEY_MULTIPLY, KEY_SUBTRACT, KEY_DIVIDE,
		KEY_BRACKET_F, KEY_BRACKET_B, KEY_LEFT_ALT, KEY_RIGHT_ALT, KEY_MINUS, KEY_PERIOD, KEY_COMMA, KEY_SLASH,
		KEY_LAST
	};

	class InputSystem : public System
	{
	public:
		InputSystem();
		virtual ~InputSystem();

		void Init() override;
		void Update(float dt) override;
		void Shutdown() override;
		void ProcessMessage(Message* message) override;

		KeyCode InputTranslateKey(int key) const;


		void InputKeyUp(KeyCode key);
		void InputKeyDown(KeyCode key);
		//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		bool IsPressed(KeyCode key) const;
		bool IsTriggered(KeyCode key);
		double GetMouseX() const;
		double GetMouseY() const;
		void SetMouseX(const double &x);
		void SetMouseY(const double &y);

		double GetOffsetX() const;
		double GetOffsetY() const;
		void SetOffsetX(const double &x);
		void SetOffsetY(const double &y);

	private:
		bool Pressed[KEY_LAST];
		bool Triggered[KEY_LAST];
		bool Check[KEY_LAST];//to check that formal one is up or down
		double mouseX;
		double mouseY;
		double scroll_offsetX;
		double scroll_offsetY;

		
	};
}

