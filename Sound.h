#ifndef SOUND_H
#define SOUND_H
#include <XAudio2.h>

class Sound : public IXAudio2VoiceCallback
{
private:
	const char* filename;
	IXAudio2SourceVoice* source_voice;
	UINT byte_data_size;
	BYTE* byte_data;
	int reference_count;

	void delete_byte_data();

protected:
	Sound();
	~Sound();
	void add_reference() {
		reference_count++;
	}
	void remove_reference() {
		reference_count--;
	}
	int get_reference_count() {
		return reference_count;
	}
	void release();

public:
	const char* get_filename() {
		return filename;
	}
	bool load(IXAudio2* audio_engine, char* filename);
	void play();
	void stop();
	void reset();

	//Sound callbacks
	void _stdcall OnVoiceProcessingPassStart(UINT32 BytesRequired);
	void _stdcall OnVoiceProcessingPassEnd();
	void _stdcall OnStreamEnd();
	void _stdcall OnBufferStart(void* pBufferContext);
	void _stdcall OnBufferEnd(void* pBufferContext);
	void _stdcall OnLoopEnd(void* pBufferContext);
	void _stdcall OnVoiceError(void* pBufferContext, HRESULT Error);

	friend class Sound_Manager;
};

#endif