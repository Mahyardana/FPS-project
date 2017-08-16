#pragma once
#include <iostream>
#include "Timer.h"
#include "SoundClass.h"
using namespace std;
class WeaponClass
{
protected:
	int MaxClipSize;
	int Remaining;
	string Name;
	int fireratems;
	float recoil;
	float animationdiffer = 0;
	virtual void animation() = 0;
	float maxanimationdiffer = 0.35;
	float bulletspeed;
	float reloadtime = 0.5f;
public:
	bool reloading = false;
	WeaponClass();
	WeaponClass(string, int, int, int, float, float);
	void setMaxclipsize(int);
	int getRemaining();
	int getMaxclipsize();
	string getName();
	void setName(string value);
	void shoot();
	void reload();
	int getFireratems();
	float getRecoil();
	float getAnimationDiffer();
	float getBulletSpeed();
	virtual float getDoorDegree() { return 0; };
};