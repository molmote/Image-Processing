/*****************************************************************************/
/*!
\file   Message.h
\author Hyeongjin Park, Dokyoom Kim, Yeongki Baek
\par    email: park.hyungjin\@digipen.edu, yeongki.baek\@digipen.edu, 
\par	email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the source file for Message Menu. 
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
namespace VK
{
	namespace MsgId
	{
		enum MessageType {
			NONE,
			QUIT,
			COLLIDE,
			TOGGLE_DEBUG_INFO,
			IMGUI_INPUT,
			PAD_INPUT,
			TOTAL_TYPE
		};
	}//namespace MsgId
	class Message
	{
	public:
		Message(MsgId::MessageType id) : MessageId(id) {};
		MsgId::MessageType MessageId;
		virtual ~Message() {};
	};
} //namespace VK