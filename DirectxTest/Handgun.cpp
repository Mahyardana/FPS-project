#include "Handgun.h"
#include "Movement.h"

Handgun_Weapon::Handgun_Weapon(string name, int remaining, int maxclipsize, int firerate, float recoil, float bulletspeed)
{
	Name = name;
	Remaining = remaining;
	MaxClipSize = maxclipsize;
	fireratems = firerate;
	this->recoil = recoil;
	this->bulletspeed = bulletspeed;
	reloadtime = 1.8;
}

void Handgun_Weapon::animation()
{
	int state = -1;
	while (animationdiffer <= 0)
	{

		animationdiffer += state*(Timer::getDifferencems() / (recoil * 100))*maxanimationdiffer;
		if (animationdiffer < -maxanimationdiffer)
		{
			Movement::deployshell();
			animationdiffer = -maxanimationdiffer;
			if (Remaining == 0)
			{
				return;
			}
			state *= -1;
		}
		Sleep(Timer::getSleepTime());
	}
	animationdiffer = 0;
}
