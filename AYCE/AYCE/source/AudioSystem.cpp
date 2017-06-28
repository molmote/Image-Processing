/*****************************************************************************/
/*!
\file   AudioSystem.cpp
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
#include "AudioSystem.h"
#include "Factory.h"


namespace VK
{

	AudioData::AudioData()
	{
		fmodChannel = nullptr;
		fmodSound = nullptr;
		volume = 0.5f;
	}
	
	AudioData::~AudioData()
	{
		fmodChannel = nullptr;
		fmodSound = nullptr;
	}

	AudioSystem::AudioSystem()
	{
		ErrorCheck(FMOD::System_Create(&m_pSystem), "Create System Error");
		ErrorCheck(m_pSystem->init(MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr), "System Initialize Error");
		//ErrorCheck(m_pSystem->getMasterChannelGroup(&m_pMasterChannel), "Master Channel Error");
		m_masterVolume = 0.5f;

	}


	AudioSystem::~AudioSystem()
	{
		
	}

	void AudioSystem::Init()
	{
		START();
		Factory::LoadAudioData("AudioData.Json");
		Create_Sound_All();
		LOG("Create All sound");
	}
	void AudioSystem::Update(float dt)
	{
		if (Fallcheck && !IsSoundPlaying(FALL_SFX))
		{
			Play_Sound(FALL_SFX, false);
			Fallcheck = false;
		}
		else
			Stop_Sound(FALL_SFX);
	}
	void AudioSystem::Shutdown()
	{
		
		Release_Sound_All();

		for (auto it : m_audioDataList)
		{
			delete it.second;
		}
		//close the fmod system
		ErrorCheck(m_pSystem->close(),"Sound Close Error");
		m_audioDataList.clear();

	}

	void AudioSystem::AddAudioDataList(AudioType aType, AudioData* aData)
	{
		m_audioDataList.emplace(aType, aData);
	}


	bool AudioSystem::Create_Sound(AudioType type)
	{
		if (m_audioDataList[type]->filename == "")
			return false;
		ErrorCheck(m_pSystem->createStream(m_audioDataList[type]->filename.c_str(), FMOD_HARDWARE, nullptr, 
								&m_audioDataList[type]->fmodSound),"Create Stream Error");

		return true;
	}
	void AudioSystem::Create_Sound_All()
	{
		for (auto it : m_audioDataList)
		{
			m_pSystem->createStream(it.second->filename.c_str(), FMOD_HARDWARE, nullptr,
				&it.second->fmodSound);
			it.second->fmodChannel->setVolume(it.second->volume);
		}
	}
	void AudioSystem::Play_Sound(AudioType type, bool repeat)
	{
		if (m_audioDataList.find(type) != m_audioDataList.end())
		{
			if (!repeat)
				m_audioDataList[type]->fmodSound->setMode(FMOD_LOOP_OFF);
			else
			{
				m_audioDataList[type]->fmodSound->setMode(FMOD_LOOP_NORMAL);
				m_audioDataList[type]->fmodSound->setLoopCount(-1);
			}

			ErrorCheck(m_pSystem->playSound(FMOD_CHANNEL_FREE, m_audioDataList[type]->fmodSound,
										false, &m_audioDataList[type]->fmodChannel),"Play Sound Error");

			float volume=0;
			m_audioDataList[type]->fmodChannel->getVolume(&volume);
			std::cout << "sound volume : "<< TranslateAudioKey(type) << "   " << volume << std::endl;
			
		}
	}
	void AudioSystem::Pause_Sound(AudioType type, bool pause)
	{
		if (m_audioDataList.find(type) != m_audioDataList.end())
			m_audioDataList[type]->fmodChannel->setPaused(pause);
	}
	void AudioSystem::Stop_Sound(AudioType type)
	{
		if (m_audioDataList.find(type) != m_audioDataList.end())
			m_audioDataList[type]->fmodChannel->stop();
	}

	void AudioSystem::Stop_Sound_All()
	{
		for (auto it : m_audioDataList)
		{
			it.second->fmodChannel->stop();
		}
	}

	void AudioSystem::Pause_Sound_All(bool pause)
	{
		for (auto it : m_audioDataList)
		{
			it.second->fmodChannel->setPaused(pause);
		}
	}

	void AudioSystem::Release_Sound(AudioType type)
	{
		m_audioDataList[type]->fmodSound->release();
		m_audioDataList.erase(type);
	}
	void AudioSystem::Release_Sound_All()
	{

		for (auto it : m_audioDataList)
		{
			it.second->fmodSound->release();
		}

	}

	void AudioSystem::VolumeUp_All()
	{
		Mute_All(false);
		m_masterVolume += 0.1f;
		if (m_masterVolume >= 1.0f)
		{
			m_masterVolume = 1.0f;
		}
		m_pMasterChannel->setVolume(m_masterVolume);
	}
	void AudioSystem::VolumeDown_All()
	{
		m_masterVolume -= 0.1f;
		if (m_masterVolume <= 0.0f)
		{
			m_masterVolume = 0.0f;
			Mute_All(true);
		}
		m_pMasterChannel->setVolume(m_masterVolume);
	}
	void AudioSystem::Mute_All(bool mute)
	{
		m_mute = mute;

		m_pMasterChannel->setMute(mute);
	}
	bool AudioSystem::IsSoundPlaying(AudioType type)
	{
		bool IsPlaying;
		m_audioDataList[type]->fmodChannel->isPlaying(&IsPlaying);
		return IsPlaying;
	}



	void AudioSystem::ErrorCheck(FMOD_RESULT result, std::string message)
	{
		if (result != FMOD_OK)
		{
			MessageBoxA(nullptr, message.c_str(), "FMOD ERROR", MB_OK);
			//exit(-1);
		}
	}

	void AudioSystem::ProcessMessage(Message* message)
	{

	}

	AudioDataList AudioSystem::GetAudioDataList() const
	{
		return m_audioDataList;
	}



	AudioType AudioSystem::TranslateAudioKey(std::string str) const
	{
		if (str == "MENU_BGM")
			return MENU_BGM;
		else if (str == "GAME_BGM")
			return GAME_BGM;
		else if (str == "OPTION_BGM")
			return OPTION_BGM;
		else if (str == "CREATE_SNOW_SFX")
			return CREATE_SNOW_SFX;
		else if (str == "WALKING_SFX")
			return WALKING_SFX;
		else if (str == "MENU_MOVE_SFX")
			return MENU_MOVE_SFX;
		else if (str == "WINNING_BGM")
			return WINNING_BGM;
		else if (str == "CREDIT_BGM")
			return CREDIT_BGM;
		else if (str == "SHOOT_SFX")
			return SHOOT_SFX;
		else if (str == "COLLISION_SFX")
			return COLLISION_SFX;
		else if (str == "WIND_SFX")
			return WIND_SFX;
		else if (str == "WINNING_SFX")
			return WINNING_SFX;
		else if (str == "FALL_SFX")
			return FALL_SFX;
		else
			return A_NONE;
	}

	std::string AudioSystem::TranslateAudioKey(AudioType type)
	{
		switch (type)
		{
		case MENU_BGM:
			return "MENU_BGM";
		case GAME_BGM:
			return "GAME_BGM";
		case OPTION_BGM:
			return "OPTION_BGM";
		case WALKING_SFX:
			return "WALKING_SFX";
		case CREATE_SNOW_SFX:
			return "CREATE_SNOW_SFX";
		case MENU_MOVE_SFX:
			return "MENU_MOVE_SFX";
		case WINNING_BGM:
			return "WINNING_BGM";
		case CREDIT_BGM:
			return "CREDIT_BGM";
		case SHOOT_SFX:
			return "SHOOT_SFX";
		case COLLISION_SFX:
			return "COLLISION_SFX";
		case WIND_SFX:
			return "WIND_SFX";
		case WINNING_SFX:
			return "WINNING_SFX";
		case FALL_SFX:
			return "FALL_SFX";
		default:
			return "None";
		}
	}

	void AudioSystem::SetVolume(AudioType type,float volume)
	{
		m_audioDataList[type]->fmodChannel->setVolume(volume);
	}
}
