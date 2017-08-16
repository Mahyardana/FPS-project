#include "WeaponClass.h"
#include <thread>
#include <Windows.h>
#include "Movement.h"
#include "Handgun.h"
#include "M4A.h"

WeaponClass::WeaponClass()
{
}

WeaponClass::WeaponClass(string name, int remaining, int maxclipsize, int firerate, float recoil, float bulletspeed)
{
	Name = name;
	Remaining = remaining;
	MaxClipSize = maxclipsize;
	fireratems = firerate;
	this->recoil = recoil;
	this->bulletspeed = bulletspeed;
}

void WeaponClass::setMaxclipsize(int value)
{
	MaxClipSize = value;
}

int WeaponClass::getRemaining()
{
	return Remaining;
}

int WeaponClass::getMaxclipsize()
{
	return MaxClipSize;
}

string WeaponClass::getName()
{
	return Name;
}

void WeaponClass::setName(string value)
{
	Name = value;
}

void WeaponClass::shoot()
{
	if (Remaining > 0 && !reloading)
	{
		Remaining--;
		thread(&WeaponClass::animation, this).detach();
	}
}

void WeaponClass::reload()
{
	if (Remaining < MaxClipSize)
	{
		reloading = true;
		float time = 0;
		float weaponpitch = 0;
		while (time < 0.5)
		{
			Movement::setWeaponPitch(weaponpitch);
			time += Timer::getDifference();
			weaponpitch = (time / 0.5) * 90;
			Sleep(Timer::getSleepTime());
		}
		float pos[] = { Movement::getX(),Movement::getY(),Movement::getZ() };
		if (Movement::getEquiped() == 0)
		{
			int channel = SoundClass::CreateChannel(SND_HANDGUN_RELOAD, true);
			if (Timer::getSlowMotionState())
				SoundClass::setSpeed(channel, 0.1);
			else
				SoundClass::setSpeed(channel, 1.0);
			SoundClass::setArgsAndPlay(channel, pos);
		}
		else
		{
			int channel = SoundClass::CreateChannel(SND_M4A_RELOAD, true);
			if (Timer::getSlowMotionState())
				SoundClass::setSpeed(channel, 0.1);
			else
				SoundClass::setSpeed(channel, 1.0);
			SoundClass::setArgsAndPlay(channel, pos);
		}
		while (time < 0.5 + reloadtime)
		{
			time += Timer::getDifference();
			Sleep(Timer::getSleepTime());
		}
		Remaining = MaxClipSize;
		animationdiffer = 0;
		while (time < 1 + reloadtime)
		{
			Movement::setWeaponPitch(weaponpitch);
			time += Timer::getDifference();
			weaponpitch = ((1 + reloadtime - time) / 0.5) * 90;
			Sleep(Timer::getSleepTime());
		}
		Movement::setWeaponPitch(0);
		reloading = false;
	}
}

int WeaponClass::getFireratems()
{
	return fireratems;
}

float WeaponClass::getRecoil()
{
	return recoil;
}

float WeaponClass::getAnimationDiffer()
{
	return animationdiffer;
}

float WeaponClass::getBulletSpeed()
{
	return bulletspeed;
}