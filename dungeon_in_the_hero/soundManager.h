#pragma once
#include "singletonBase.h"

// fmod 관련 파일
#include "inc/fmod.hpp"

#pragma comment(lib, "lib/fmodex_vc.lib")

#define EXTRA_CHANNEL_BUFFER	5
#define SOUND_BUFFER			10
#define TOTAL_SOUND_BUFFER		SOUND_BUFFER + EXTRA_CHANNEL_BUFFER

using namespace FMOD;

class soundManager : public singletonBase<soundManager>
{
private:
	FMOD::System*	m_system;
	FMOD::Sound**	m_sound;
	FMOD::Channel**	m_channel;

	map<string, FMOD::Sound**>	m_mapSounds;

	//map<string, FMOD::Sound**>::iterator	m_iterSound;

	//map<string, FMOD::Channel**>	m_mapChannels;
	//map<string, FMOD::Channel**>::iterator	m_iterChannel;

public:
	HRESULT init();
	void release();
	void update();

	void addSound(string soundName, bool isBGM, bool isLoop);
	void play(string soundName, float volume = 1.0f);	// 0.0f ~ 1.0f
	void stop(string soundName);
	void pause(string soundName);
	void resume(string soundName);

	void setVolume(string soundName, float volume);

	bool isPlaying(string soundName);
	bool isPause(string soundName);

	soundManager();
	~soundManager();
};

