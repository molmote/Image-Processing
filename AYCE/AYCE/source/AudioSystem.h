/*****************************************************************************/
/*!
\file   AudioSystem.h
\author Dokyoon Kim
\par    email: dokyoon.kim\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for AudioSystem features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#pragma once
#include "precompiled.h"
#include "System.h"
#include <fmod.hpp>

namespace VK
{
	enum AudioType
	{
		A_NONE,
		MENU_BGM,
		GAME_BGM,
		OPTION_BGM,
		CREATE_SNOW_SFX,
		WALKING_SFX,
		MENU_MOVE_SFX,
		WINNING_BGM,
		CREDIT_BGM,
		SHOOT_SFX,
		COLLISION_SFX,
		WIND_SFX,
		WINNING_SFX,
		FALL_SFX
	};

	struct AudioData
	{
		AudioData();
		~AudioData();
		std::string filename;
		FMOD::Channel* fmodChannel;
		FMOD::Sound* fmodSound;
		float volume;
	};

	typedef std::map<AudioType, AudioData*> AudioDataList;

	
	typedef std::map<int, FMOD::Channel*> FMODChannelList;
	typedef std::map<int, FMOD::Sound*> FMODSoundList;
	

	class AudioSystem : public System
	{
	public:
		AudioSystem();
		virtual ~AudioSystem();

		void Init() override;
		void Update(float dt) override;
		void Shutdown() override;
		void ProcessMessage(Message* message) override;


		bool Create_Sound(AudioType type);
		void Create_Sound_All();
		void Play_Sound(AudioType type, bool repeat);
		void Pause_Sound(AudioType type, bool pause);
		void Stop_Sound(AudioType type);
		void Stop_Sound_All();
		void Pause_Sound_All(bool pause);
		void Release_Sound(AudioType type);

		void Release_Sound_All();

		void SetVolume(AudioType type, float volume);
		void VolumeUp_All();
		void VolumeDown_All();
		void Mute_All(bool mute);
		bool IsMute() const { return m_mute; };
		bool IsSoundPlaying(AudioType type);

		AudioDataList GetAudioDataList() const;

		void AddAudioDataList(AudioType aType, AudioData* aData);
		AudioType TranslateAudioKey(std::string str) const;
		std::string TranslateAudioKey(AudioType type);


		bool Fallcheck = false;
		//bool Create

	private:
		AudioDataList m_audioDataList;		
		FMOD::System* m_pSystem;
		const static int MAX_CHANNELS = 100;

		void ErrorCheck(FMOD_RESULT result, std::string message);

		FMOD::ChannelGroup* m_pMasterChannel;
		float m_masterVolume;
		

		bool m_mute;



	};
}

