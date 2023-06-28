#include "Audio.h"
#include <map>

// Initialize static member
std::map<std::string, Audio*> Audio::sAudiosLoaded;

Audio::Audio()
{
	sample = 0;
}

Audio::~Audio()
{
	if (sample != 0)
	{
		BASS_SampleFree(sample);
		sample = 0;
	}
}

HCHANNEL Audio::play(float volume)
{
	HCHANNEL channel = BASS_SampleGetChannel(sample, false);
	if (channel != 0)
	{
		BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, volume);
		BASS_ChannelPlay(channel, true);
	}
	return channel;
}

bool Audio::Init()
{
	// Initialize BASS
	if (BASS_Init(-1, 44100, 0, nullptr, nullptr))
		return true;
	else
		return false;
}

Audio* Audio::Get(const char* filename)
{
	// Check if the audio is already loaded
	auto it = sAudiosLoaded.find(filename);
	if (it != sAudiosLoaded.end())
	{
		// Audio already loaded, return the instance
		return it->second;
	}
	else
	{
		// Audio not loaded, load it and store it in the map
		Audio* audio = new Audio();
		audio->sample = BASS_SampleLoad(FALSE, filename, 0, 0, 3, BASS_SAMPLE_OVER_POS);
		if (audio->sample != 0)
		{
			sAudiosLoaded[filename] = audio;
			return audio;
		}
		else
		{
			delete audio;
			return nullptr;
		}
	}
}


HCHANNEL Audio::Play(const char* filename, float volume, bool loop)
{
	Audio* audio = Get(filename);
	if (audio != nullptr)
	{
		HCHANNEL channel = audio->play(volume);
		if (channel != 0)
		{
			BASS_ChannelFlags(channel, loop ? BASS_SAMPLE_LOOP : 0, BASS_SAMPLE_LOOP);
		}
		return channel;
	}
	else
		return 0;
}

HCHANNEL Audio::Play3D(const char* filename, Vector3 position, float volume, bool loop)
{
	Audio* audio = Get(filename);
	if (audio != nullptr)
	{
		HCHANNEL channel = audio->play(volume);
		if (channel != 0)
		{
			BASS_3DVECTOR bassPosition;
			bassPosition.x = position.x;
			bassPosition.y = position.y;
			bassPosition.z = position.z;
			BASS_ChannelSet3DAttributes(channel, BASS_3DMODE_NORMAL, 0, 0, 0, 0, 0);
			BASS_ChannelSet3DPosition(channel, &bassPosition, nullptr, nullptr);
			BASS_ChannelFlags(channel, loop ? BASS_SAMPLE_LOOP : 0, BASS_SAMPLE_LOOP);
		}
		return channel;
	}
	else
		return 0;
}


bool Audio::Stop(HCHANNEL channel)
{
	return BASS_ChannelStop(channel);
}
