#include "SoundClass.h"

SoundClass* SoundClass::Instance = NULL;

SoundClass::SoundClass()
{
	FMOD::System_Create(&soundsystem);
	soundsystem->init(2048, NULL, NULL);
}

SoundClass::~SoundClass()
{

}

SoundClass * SoundClass::getInstance()
{
	if (Instance == NULL)
	{
		Instance = new SoundClass();
	}
	return Instance;
}

void SoundClass::CreateSound(char * filename, bool Loop)
{
	SoundClass* instance = getInstance();
	FMOD::Sound* sound;
	if (!Loop)
		instance->soundsystem->createSound(filename, FMOD_LOOP_OFF | FMOD_3D, NULL, &sound);
	else
		instance->soundsystem->createSound(filename, FMOD_LOOP_NORMAL | FMOD_3D, NULL, &sound);
	instance->sounds.push_back(sound);
}

int SoundClass::CreateChannel(int id, bool boundtocharacter)
{
	SoundClass* instance = getInstance();
	instance->cleanchannels();
	FMOD::Channel* channel;
	instance->soundsystem->playSound(instance->sounds[id], NULL, 1, &channel);
	if (boundtocharacter)
		channel->set3DLevel(0);
	instance->channels.push_back(channel);
	return instance->channels.size() - 1;
}

void SoundClass::setVolume(int id, float value)
{
	SoundClass* instance = getInstance();
	instance->channels[id]->setVolume(value);
}

void SoundClass::setSpeed(int id, float value)
{
	SoundClass* instance = getInstance();
	instance->channels[id]->setPitch(value);
}

void SoundClass::updatesystem()
{
	getInstance()->soundsystem->update();
}

void SoundClass::cleanchannels()
{
	int i = 0;
	while (i < channels.size())
	{
		bool state;
		channels[i]->getPaused(&state);
		if (state)
		{
			channels[i]->stop();
			channels.erase(channels.begin() + i);
			i--;
		}
		i++;
	}
}

void SoundClass::setListener(const float* Position, const float* Oriention)
{
	SoundClass* instance = getInstance();
	FMOD_VECTOR pos = { Position[0],Position[1],Position[2] };
	FMOD_VECTOR forward = { Oriention[0],Oriention[1],Oriention[2] };
	FMOD_VECTOR up = { Oriention[3],Oriention[4],Oriention[5] };
	instance->soundsystem->set3DListenerAttributes(0, &pos, NULL, &forward, &up);
}

void SoundClass::setArgsAndPlay(int id, const float* Position)
{
	SoundClass* instance = getInstance();
	FMOD_VECTOR pos = { Position[0],Position[1],Position[2] };
	instance->channels[id]->set3DAttributes(&pos, NULL);
	instance->channels[id]->setPaused(0);
}