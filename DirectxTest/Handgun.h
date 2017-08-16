#pragma once
#include "WeaponClass.h"
class Handgun_Weapon :public WeaponClass
{
public:
	Handgun_Weapon(string name, int remaining, int maxclipsize, int firerate, float recoil, float bulletspeed);
private:
	void animation();
};