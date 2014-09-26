#include "Sound_Manager.h"

Sound_Manager::Sound_Manager()
{
	audio_system = NULL;
	mastering_voice = NULL;
}

Sound_Manager::~Sound_Manager()
{
	release();
}

bool Sound_Manager::initialise()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	UINT32 int_flags = 0;

	//Create XAudio2 system
	if(FAILED(XAudio2Create(&audio_system, int_flags)))
	{
		return FALSE;
	}

	//Create mastering voice
	if(FAILED(audio_system->CreateMasteringVoice(&mastering_voice)))
	{
		if(audio_system)
		{
			audio_system->Release();
			audio_system = NULL;
		}
		return FALSE;
	}

	//Start the audio engine
	audio_system->StartEngine();
	return TRUE;
}

bool Sound_Manager::load(char* filename)
{
	if(filename == 0)
	{
		return FALSE;
	}

	if(get_sound(filename) != NULL)
	{
		return TRUE;
	}

	Sound* temp_sound = new Sound();

	if(temp_sound->load(audio_system, filename))
	{
		std::string filename_string = filename;
		sound_queue[filename_string] = temp_sound;
		temp_sound = NULL;
		return TRUE;
	}
	else
	{
		delete temp_sound;
		temp_sound = NULL;
		return FALSE;
	}
}

Sound* Sound_Manager::get_sound(char* filename)
{
	std::string filename_string = filename;

	sound_map::const_iterator search_result = sound_queue.find(filename_string);

	if(search_result != sound_queue.end())
	{
		search_result->second->add_reference();
		return search_result->second;
	}

	return NULL;
}

void Sound_Manager::release_sound(Sound* sound)
{
	if(sound)
	{
		sound->remove_reference();
		if(sound->get_reference_count() <= 0)
		{
			std::string filename_string = sound->get_filename();
			sound_map::const_iterator search_result = sound_queue.find(filename_string);
			if(search_result != sound_queue.end())
			{
				sound_queue.erase(search_result);
			}
			delete sound;
			sound = NULL;
		}
	}
}

void Sound_Manager::release()
{
	if(!sound_queue.empty())
	{
		sound_map::iterator map_iterator;
		for(map_iterator = sound_queue.begin(); map_iterator != sound_queue.end(); map_iterator++)
		{
			map_iterator->second->release();
			map_iterator->second = NULL;
		}

		sound_queue.clear();
	}
}