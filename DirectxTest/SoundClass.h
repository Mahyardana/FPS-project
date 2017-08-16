#pragma once

#define SND_BG 0
#define SND_FOOTSTEP 1
#define SND_HANDGUN 2
#define SND_M4A 3
#define SND_BOUNCE1 4
#define SND_BOUNCE2 5
#define SND_BOUNCE3 6
#define SND_HANDGUN_RELOAD 7
#define SND_M4A_RELOAD 8
#define SND_HIT 9


#include <iostream>
#include <fmod.hpp>
#include <vector>

class SoundClass
{
private:
	static SoundClass* Instance;
	std::vector<FMOD::Sound*> sounds;
	std::vector<FMOD::Channel*> channels;
	SoundClass();
	~SoundClass();
	static SoundClass* getInstance();
	FMOD::System* soundsystem;
public:
	static void setListener(const float* Position, const float* Oriention);
	static void setArgsAndPlay(int id, const float*);
	static void CreateSound(char* filename, bool Loop);
	static int CreateChannel(int id, bool boundtocharacter);
	static void setVolume(int id, float value);
	static void setSpeed(int id, float value);
	static void updatesystem();
	void cleanchannels();
};