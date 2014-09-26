#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <map>
#include <string>
#include <xaudio2.h>
#include "Sound.h"

class Sound_Manager
{
private:
	typedef std::map<std::string, Sound*> sound_map;
	sound_map sound_queue;
	IXAudio2* audio_system;
	IXAudio2MasteringVoice* mastering_voice;

public:
	Sound_Manager();
	~Sound_Manager();
	bool initialise();
	bool load(char* filename);
	Sound* get_sound(char* filename);
	void release_sound(Sound* sound);
	void release();
};

#endif