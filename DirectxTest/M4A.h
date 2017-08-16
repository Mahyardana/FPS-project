#pragma once
#include "WeaponClass.h"
class M4A_Weapon :public WeaponClass
{
public:
	M4A_Weapon(string name, int remaining, int maxclipsize, int firerate, float recoil, float bulletspeed);
	float getDoorDegree();
private:
	int numberofanimationthreads = 0;
	float doordegree = 360;
	void animation();
};