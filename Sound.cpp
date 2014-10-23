#include "Sound.h"
#include "WaveFile.h"

Sound::Sound() 
{
	source_voice = NULL;
	byte_data_size = 0;
	byte_data = NULL;
	reference_count = 0;
}

Sound::~Sound()
{
	release();
}

void Sound::release()
{
	//Delete voice
	if(source_voice)
	{
		source_voice->DestroyVoice();
		source_voice = NULL;
	}

	delete_byte_data();
}

void Sound::delete_byte_data() 
{
	//Delete sound data
	if(byte_data)
	{
		delete[] byte_data;
		byte_data = NULL;
	}
}

bool Sound::load(IXAudio2* audio_engine, std::string filename) 
{
	//Load wave file
	WaveFile wave_file;
	if(FAILED(wave_file.Open(const_cast<char*>(filename.c_str()), NULL, WAVEFILE_READ)))
	{
		return FALSE;
	}

	//Get Format of wave file
	WAVEFORMATEX* pwfx = wave_file.GetFormat();

	//Calculate number of bytes in the loaded file
	byte_data_size = wave_file.GetSize();

	//Read data into memory byte by byte
	byte_data = new BYTE[byte_data_size];
	DWORD bytes_read;
	if(FAILED(wave_file.Read(byte_data, byte_data_size, &bytes_read)))
	{
		return FALSE;
	}

	//Create source voice
	if(FAILED(audio_engine->CreateSourceVoice(&source_voice, pwfx, 0U, 2.0f, this)))
	{
		//If files delete sound data
		delete_byte_data();
		return FALSE;
	}

	//If success set file name and return
	this->filename = filename;
	return TRUE;
}

void Sound::play()
{
	//Get state of Source Voice
	XAUDIO2_VOICE_STATE state;
	source_voice->GetState(&state);
	//If nothing is queued to play then
	if(state.BuffersQueued <= 0)
	{
		//Set sound to play again from the start
		reset();
	}
	//And start the voice playing
	source_voice->Start(0);
}

void Sound::stop()
{
	//Get state of Source Voice
	XAUDIO2_VOICE_STATE state;
	source_voice->GetState(&state);
	//If something is queued to play then
	if(state.BuffersQueued > 0)
	{
		//Stop it
		source_voice->Stop();
	}
}

void Sound::reset()
{
	XAUDIO2_BUFFER buffer = {0};
	buffer.pAudioData = byte_data;
	buffer.Flags = XAUDIO2_END_OF_STREAM; //Tell the source not to expect any data after this buffer
	buffer.AudioBytes = byte_data_size;

	source_voice->Stop(); //Stop the source buffer from trying to play any data, so we can load into it
	source_voice->SubmitSourceBuffer(&buffer);
}

void _stdcall Sound::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
}

void _stdcall Sound::OnVoiceProcessingPassEnd()
{
}

void _stdcall Sound::OnStreamEnd()
{
}

void _stdcall Sound::OnBufferStart(void* pBufferContext)
{
}

void _stdcall Sound::OnBufferEnd(void* pBufferContext)
{
}

void _stdcall Sound::OnLoopEnd(void* pBufferContext)
{
}

void _stdcall Sound::OnVoiceError(void* pBufferContext, HRESULT Error)
{
}