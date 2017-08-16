#include "M4A.h"
#include "Movement.h"
M4A_Weapon::M4A_Weapon(string name, int remaining, int maxclipsize, int firerate, float recoil, float bulletspeed)
{
	Name = name;
	Remaining = remaining;
	MaxClipSize = maxclipsize;
	fireratems = firerate;
	this->recoil = recoil;
	this->bulletspeed = bulletspeed;
	numberofanimationthreads = 0;
	doordegree = 0;
	reloadtime = 3.8;
}

float M4A_Weapon::getDoorDegree()
{
	return doordegree;
}

void M4A_Weapon::animation()
{
	int threadnumber = numberofanimationthreads;
	numberofanimationthreads++;
	int state = -1;
	shell* shellinfo;
	animationdiffer = 0;
	int rotatespeed = 1;
	while (doordegree <= 0)
	{
		if (numberofanimationthreads > threadnumber + 1)
		{
			return;
		}
		if (state == -1)
			animationdiffer += state*(Timer::getDifferencems() / (recoil * 100))*maxanimationdiffer;
		if (animationdiffer < -maxanimationdiffer)
		{
			shellinfo = Movement::deployshell();
			doordegree -= shellinfo->speed * 36;
			if (doordegree < -180)
				doordegree = -180;
			animationdiffer = 0;
			state *= -1;
		}
		if (doordegree < 0)
		{
			doordegree += rotatespeed * Timer::getDifference();
			rotatespeed += 10000 * Timer::getDifference();
		}
		Sleep(Timer::getSleepTime());
	}
	doordegree = 0;
	animationdiffer = 0;
}
