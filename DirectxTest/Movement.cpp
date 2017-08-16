#include "Movement.h"
#include <iostream>
#include <D3DX10math.h>
#include <thread>

Movement* Movement::Instance = nullptr;

void Movement::bulletAnimation(bullet* input)
{
	float speed = weapons[input->weaponindex]->getBulletSpeed();
	float maxbulletstep = this->maxbulletstep;
	while (input->step <= maxbulletstep && !input->isjobdone)
	{
		float time = Timer::getDifference();
		float bspeed = speed*time;
		input->step += bspeed;
		Sleep(Timer::getSleepTime());
	}
	input->isjobdone = true;
	return;
}

void Movement::changeWeaponAnimation()
{
	if (!ChangingWeapon && !getRealoadingState())
	{
		ChangingWeapon = true;
		float time = 0;
		float weaponpitch = 0;
		while (time < 0.5)
		{
			Movement::setWeaponPitch(weaponpitch);
			time += Timer::getDifference();
			weaponpitch = (time / 0.5) * 90;
			Sleep(Timer::getSleepTime());
		}
		if (this->equiped == this->weapons.size() - 1)
			this->equiped = 0;
		else
			this->equiped++;
		while (time < 1)
		{
			Movement::setWeaponPitch(weaponpitch);
			time += Timer::getDifference();
			weaponpitch = ((1 - time) / 0.5) * 90;
			Sleep(Timer::getSleepTime());
		}
		Movement::setWeaponPitch(0);
		ChangingWeapon = false;
	}

}

void Movement::shellAnimation(shell* input)
{
	float xspeed = input->speed*cos(D3DXToRadian(input->primarydeparturedegree));
	float yspeed = input->speed*sin(D3DXToRadian(input->primarydeparturedegree));
	float t = 0;
	while (!input->isjobdone&&input->y >= 0)
	{
		float time = Timer::getDifference();
		input->roll += input->rollrotatespeed*time;
		input->selfyaw += input->yawrotatespeed*time;
		if (input->roll > 360)
			input->roll -= 360;
		if (input->selfyaw > 360)
			input->selfyaw -= 360;
		float v = -9.8*t + yspeed;
		input->stepx += time*xspeed;
		input->y += time*v;
		t += time;
		Sleep(Timer::getSleepTime());
	}
	int index;
	uniform_int_distribution<> dis(0, 2);
	index = dis(mt);
	float position[] = { input->x,input->y,input->z };
	if (index == 0)
	{
		SoundClass::setArgsAndPlay(SoundClass::CreateChannel(SND_BOUNCE1, false), position);
	}
	else if (index == 1)
	{
		SoundClass::setArgsAndPlay(SoundClass::CreateChannel(SND_BOUNCE2, false), position);
	}
	else
	{
		SoundClass::setArgsAndPlay(SoundClass::CreateChannel(SND_BOUNCE3, false), position);
	}
	input->isjobdone = true;
	return;
}

Movement::Movement()
{

}

Movement * Movement::getInstance()
{
	if (Instance)
	{
		return Instance;
	}
	else
	{
		Instance = new Movement();
		Instance->mt = mt19937(Instance->random());
		WeaponClass* w1 = new Handgun_Weapon("Handgun", 11, 11, 500, 0.5f, 45);
		WeaponClass* w2 = new M4A_Weapon("M4A", 30, 30, 100, 0.2f, 100);
		Instance->weapons.push_back(w1);
		Instance->weapons.push_back(w2);
		Instance->forwardzblocked = false;
		Instance->forwardxblocked = false;
		Instance->backwardzblocked = false;
		Instance->backwardxblocked = false;
		Instance->leftzblocked = false;
		Instance->leftxblocked = false;
		Instance->rightzblocked = false;
		Instance->rightxblocked = false;
		return Instance;
	}
}

shell* Movement::deployshell()
{
	Movement* instance = getInstance();
	shell *temp = new shell;
	uniform_int_distribution<> dis(130, 150);
	temp->isjobdone = 0;
	temp->primarydeparturedegree = dis(instance->mt);
	dis = uniform_int_distribution<>(500, 800);
	temp->rollrotatespeed = dis(instance->mt);
	//temp->rollrotatespeed = 0;
	dis = uniform_int_distribution<>(3, 5);
	temp->speed = dis(instance->mt);
	temp->stepx = 0;
	temp->stepy = 0;
	temp->weaponindex = instance->equiped;
	temp->x = instance->x;
	temp->y = instance->y;
	temp->z = instance->z;
	temp->yaw = instance->yaw;
	dis = uniform_int_distribution<>(500, 800);
	temp->yawrotatespeed = dis(instance->mt);
	temp->pitch = instance->pitch;
	temp->roll = 0;
	dis = uniform_int_distribution<>(-30, 30);
	temp->selfpitch = dis(instance->mt);
	dis = uniform_int_distribution<>(-30, 30);
	temp->selfyaw = dis(instance->mt);
	instance->shells.push_back(temp);
	thread(&Movement::shellAnimation, instance, temp).detach();
	return temp;
}

void Movement::move_forward()
{
	Movement* instance = getInstance();
	if (!instance->forwardzblocked)
		instance->z += cosf(D3DXToRadian(instance->yaw))*instance->speed*Timer::getDifference();
	if (!instance->forwardxblocked)
		instance->x += sinf(D3DXToRadian(instance->yaw))*instance->speed*Timer::getDifference();
}

void Movement::sprint_forward()
{
	Movement* instance = getInstance();
	if (!instance->forwardzblocked)
		instance->z += cosf(D3DXToRadian(instance->yaw))*instance->speed * 2 * Timer::getDifference();
	if (!instance->forwardxblocked)
		instance->x += sinf(D3DXToRadian(instance->yaw))*instance->speed * 2 * Timer::getDifference();
}

void Movement::move_backward()
{
	Movement* instance = getInstance();
	if (!instance->backwardzblocked)
		instance->z -= cosf(D3DXToRadian(instance->yaw))*instance->speed*Timer::getDifference();
	if (!instance->backwardxblocked)
		instance->x -= sinf(D3DXToRadian(instance->yaw))*instance->speed*Timer::getDifference();
}

void Movement::move_left()
{
	Movement* instance = getInstance();
	if (!instance->leftzblocked)
		instance->z -= cosf(D3DXToRadian(90 + instance->yaw))*instance->speed*Timer::getDifference();
	if (!instance->leftxblocked)
		instance->x -= sinf(D3DXToRadian(90 + instance->yaw))*instance->speed*Timer::getDifference();
}

void Movement::move_right()
{
	Movement* instance = getInstance();
	if (!instance->rightzblocked)
		instance->z += cosf(D3DXToRadian(90 + instance->yaw))*instance->speed*Timer::getDifference();
	if (!instance->rightxblocked)
		instance->x += sinf(D3DXToRadian(90 + instance->yaw))*instance->speed*Timer::getDifference();
}

float Movement::getSpeed()
{
	Movement* instance = getInstance();
	return instance->speed*Timer::getDifference();
}

float Movement::getX()
{
	Movement* instance = getInstance();
	return instance->x;
}

float Movement::getY()
{
	Movement* instance = getInstance();
	return instance->y;
}

float Movement::getZ()
{
	Movement* instance = getInstance();
	return instance->z;
}

int Movement::getOriention()
{
	Movement* instance = getInstance();
	return instance->oriention;
}

void Movement::changeOriention()
{
	Movement* instance = getInstance();
	instance->oriention *= -1;
}

void Movement::setRecoilDiffer(float differ)
{
	Movement* instance = getInstance();
	instance->recoildiffer = differ;
}

float Movement::getRecoilDiffer()
{
	Movement* instance = getInstance();
	return instance->recoildiffer;
}

void Movement::look_pitch(float value)
{
	Movement* instance = getInstance();
	instance->lastframepitch = instance->pitch;
	if (instance->pitch <= 70 && instance->pitch >= -80)
		instance->pitch += value*instance->mousespeed*instance->speed*Timer::getDifference();
	if (instance->pitch > 70)
	{
		instance->pitch = 70;
	}
	else if (instance->pitch < -80)
	{
		instance->pitch = -80;
	}
	if (isnan(instance->pitch))
		instance->pitch = instance->lastframepitch;
}

void Movement::look_yaw(float value)
{
	Movement* instance = getInstance();
	instance->lastframeyaw = instance->yaw;
	instance->yaw += value*instance->mousespeed*instance->speed*Timer::getDifference();
	if (instance->yaw > 360)
	{
		instance->yaw -= 360;
	}
	else if (instance->yaw < -360)
	{
		instance->yaw += 360;
	}
}

float Movement::getPitch()
{
	Movement* instance = getInstance();
	return instance->pitch;
}

float Movement::getYaw()
{
	Movement* instance = getInstance();
	return instance->yaw;
}

float Movement::getPitchDiffer()
{
	Movement* instance = getInstance();
	return fabs(instance->lastframepitch - instance->pitch);
}

float Movement::getYawDiffer()
{
	Movement* instance = getInstance();
	return fabs(instance->lastframeyaw - instance->yaw);
}

void Movement::nextWeapon()
{
	Movement* instance = getInstance();
	thread(&Movement::changeWeaponAnimation, instance).detach();
}

void Movement::previousWeapon()
{
	Movement* instance = getInstance();
	if (instance->equiped == 0)
		instance->equiped = instance->weapons.size() - 1;
	else
		instance->equiped--;
}

void Movement::shoot()
{
	Movement* instance = getInstance();
	bullet* newbullet = new bullet;
	newbullet->isjobdone = false;
	newbullet->pitch = instance->pitch;
	newbullet->yaw = instance->yaw;
	newbullet->weaponindex = instance->equiped;
	newbullet->step = 0;
	newbullet->x = instance->x;
	newbullet->y = instance->y;
	newbullet->z = instance->z;
	float position[] = { instance->x,instance->y,instance->z };
	if (instance->equiped == 0)
	{
		int channel = SoundClass::CreateChannel(SND_HANDGUN, true);
		if (Timer::getSlowMotionState())
			SoundClass::setSpeed(channel, 0.1);
		else
			SoundClass::setSpeed(channel, 1.0);
		SoundClass::setArgsAndPlay(channel, position);
	}
	else
	{
		int channel = SoundClass::CreateChannel(SND_M4A, true);
		if (Timer::getSlowMotionState())
			SoundClass::setSpeed(channel, 0.1);
		else
			SoundClass::setSpeed(channel, 1.0);
		SoundClass::setArgsAndPlay(channel, position);
	}
	instance->bullets.push_back(newbullet);
	thread(&Movement::bulletAnimation, instance, newbullet).detach();
	instance->weapons[instance->equiped]->shoot();
}

void Movement::reload()
{
	Movement* instance = getInstance();
	thread(&WeaponClass::reload, instance->weapons[instance->equiped]).detach();
}

int Movement::getEquiped()
{
	Movement* instance = getInstance();
	return instance->equiped;
}

float Movement::getSlideDiffer()
{
	Movement* instance = getInstance();
	return instance->weapons[instance->equiped]->getAnimationDiffer();
}

void Movement::refreshScene()
{
	Movement* instance = getInstance();
	instance->isrefreshing = !instance->isrefreshing;
}

bool Movement::getRefreshState()
{
	Movement* instance = getInstance();
	return instance->isrefreshing;
}

vector<WeaponClass*>& Movement::getWeapons()
{
	Movement* instance = getInstance();
	return instance->weapons;
}

vector<bullet*>& Movement::getBullets()
{
	Movement* instance = getInstance();
	return instance->bullets;
}

vector<shell*>& Movement::getShells()
{
	Movement* instance = Movement::getInstance();
	return instance->shells;
}

void Movement::checksurroundings(vector<ModelClass*> *models)
{
	Movement* instance = getInstance();
	instance->forwardzblocked = false;
	instance->forwardxblocked = false;
	instance->backwardzblocked = false;
	instance->backwardxblocked = false;
	instance->leftzblocked = false;
	instance->leftxblocked = false;
	instance->rightzblocked = false;
	instance->rightxblocked = false;
	float forwardz = cosf(D3DXToRadian(instance->yaw))*instance->speed*Timer::getDifference();
	float forwardx = sinf(D3DXToRadian(instance->yaw))*instance->speed*Timer::getDifference();
	float backwardz = -cosf(D3DXToRadian(instance->yaw))*instance->speed*Timer::getDifference();
	float backwardx = -sinf(D3DXToRadian(instance->yaw))*instance->speed*Timer::getDifference();
	float leftz = -cosf(D3DXToRadian(90 + instance->yaw))*instance->speed*Timer::getDifference();
	float leftx = -sinf(D3DXToRadian(90 + instance->yaw))*instance->speed*Timer::getDifference();
	float rightz = cosf(D3DXToRadian(90 + instance->yaw))*instance->speed*Timer::getDifference();
	float rightx = sinf(D3DXToRadian(90 + instance->yaw))*instance->speed*Timer::getDifference();
	float positionX = 0, positionY = 0, positionZ = 0, sizeX = 0, sizeY = 0, sizeZ = 0;
	for (int i = 0; i < models->size(); i++)
	{
		models->at(i)->GetData(&positionX, &positionY, &positionZ, &sizeX, &sizeY, &sizeZ);
		if (!models->at(i)->isbreaked && models->at(i)->isbreakable)
		{
			if (!instance->forwardxblocked &&
				forwardx + instance->x <= positionX + sizeX / 2 &&
				forwardx + instance->x >= positionX - sizeX / 2 &&
				instance->z <= positionZ + sizeZ / 2 &&
				instance->z >= positionZ - sizeZ / 2)
				instance->forwardxblocked = true;
			if (!instance->forwardzblocked &&
				forwardz + instance->z <= positionZ + sizeZ / 2 &&
				forwardz + instance->z >= positionZ - sizeZ / 2 &&
				instance->x <= positionX + sizeX / 2 &&
				instance->x >= positionX - sizeX / 2)
				instance->forwardzblocked = true;
			if (!instance->backwardxblocked &&
				backwardx + instance->x <= positionX + sizeX / 2 &&
				backwardx + instance->x >= positionX - sizeX / 2 &&
				instance->z <= positionZ + sizeZ / 2 &&
				instance->z >= positionZ - sizeZ / 2)
				instance->backwardxblocked = true;
			if (!instance->backwardzblocked &&
				backwardz + instance->z <= positionZ + sizeZ / 2 &&
				backwardz + instance->z >= positionZ - sizeZ / 2 &&
				instance->x <= positionX + sizeX / 2 &&
				instance->x >= positionX - sizeX / 2)
				instance->backwardzblocked = true;
			if (!instance->leftxblocked &&
				leftx + instance->x <= positionX + sizeX / 2 &&
				leftx + instance->x >= positionX - sizeX / 2 &&
				instance->z <= positionZ + sizeZ / 2 &&
				instance->z >= positionZ - sizeZ / 2)
				instance->leftxblocked = true;
			if (!instance->leftzblocked &&
				leftz + instance->z <= positionZ + sizeZ / 2 &&
				leftz + instance->z >= positionZ - sizeZ / 2 &&
				instance->x <= positionX + sizeX / 2 &&
				instance->x >= positionX - sizeX / 2)
				instance->leftzblocked = true;
			if (!instance->rightxblocked &&
				rightx + instance->x <= positionX + sizeX / 2 &&
				rightx + instance->x >= positionX - sizeX / 2 &&
				instance->z <= positionZ + sizeZ / 2 &&
				instance->z >= positionZ - sizeZ / 2)
				instance->rightxblocked = true;
			if (!instance->rightzblocked &&
				rightz + instance->z <= positionZ + sizeZ / 2 &&
				rightz + instance->z >= positionZ - sizeZ / 2 &&
				instance->x <= positionX + sizeX / 2 &&
				instance->x >= positionX - sizeX / 2)
				instance->rightzblocked = true;
		}
	}
}

float Movement::getWeaponPitch()
{
	Movement* instance = getInstance();
	return instance->WeaponPitch;
}

void Movement::setWeaponPitch(float value)
{
	Movement* instance = getInstance();
	instance->WeaponPitch = value;
}

bool Movement::getRealoadingState()
{
	Movement* instance = getInstance();
	return instance->weapons[instance->equiped]->reloading;
}

bool Movement::getChangingState()
{
	Movement* instance = getInstance();
	return instance->ChangingWeapon;
}
