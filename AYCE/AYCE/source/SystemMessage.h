/*****************************************************************************/
/*!
\file   Message.h
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Message features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
namespace VK
{

	namespace SMid
	{
		enum MessageIdType
		{
			SM_NONE,
			SM_COLLISION
			//SM
		};
	}
	class Message
	{
	public:
		Message(SMid::MessageIdType id) : MessageId(id) {};
		virtual ~Message() {};
		SMid::MessageIdType MessageId;
	};
}