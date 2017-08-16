#pragma once
#include "M4A.h"
#include "Handgun.h"
#include <vector>
#include "Timer.h"
#include "ModelClass.h"
#include <random>
#include "SoundClass.h"
struct bullet
{
	float pitch;
	float yaw;
	float step;
	int weaponindex;
	bool isjobdone = 0;
	float x, y, z;
};
struct shell
{
	float pitch;
	float yaw;
	float selfyaw;
	float selfpitch;
	float yawrotatespeed;
	float primarydeparturedegree;
	float roll;
	float stepy;
	float stepx;
	float rollrotatespeed;
	int weaponindex;
	bool isjobdone = 0;
	float x, y, z;
	float speed;
};
class Movement
{
private:
	Movement();
	static Movement* getInstance();
	static Movement* Instance;
	float x = 0;
	float y = 5;
	float z = -10;
	float lastframex = 0;
	float lastframey = 5;
	float lastframez = -10;
	float speed = 10;
	float mousespeed = 1;
	float yaw = 0;
	float pitch = 0;
	float lastframeyaw = 0;
	float lastframepitch = 0;
	int equiped = 0;
	int oriention = 1;
	bool forwardzblocked;
	bool forwardxblocked;
	bool backwardzblocked;
	bool backwardxblocked;
	bool leftzblocked;
	bool leftxblocked;
	bool rightzblocked;
	bool rightxblocked;
	float recoildiffer = 0;
	void bulletAnimation(bullet*);
	void changeWeaponAnimation();
	bool ChangingWeapon = false;
	void shellAnimation(shell*);
	int maxbulletstep = 100;
	vector<WeaponClass*> weapons;
	vector<bullet*> bullets;
	vector<shell*> shells;
	random_device random;
	mt19937 mt;
	bool isrefreshing = false;
	float WeaponPitch = 0;
public:
	static shell* deployshell();
	static void move_forward();
	static void sprint_forward();
	static void move_backward();
	static void move_left();
	static void move_right();
	static float getSpeed();
	static float getX();
	static float getY();
	static float getZ();
	static int getOriention();
	static void changeOriention();
	static void setRecoilDiffer(float);
	static float getRecoilDiffer();
	static void look_pitch(float value);
	static void look_yaw(float value);
	static float getPitch();
	static float getYaw();
	static float getPitchDiffer();
	static float getYawDiffer();
	static void nextWeapon();
	static void previousWeapon();
	static void shoot();
	static void reload();
	static int getEquiped();
	static float getSlideDiffer();
	static void refreshScene();
	static bool getRefreshState();
	static vector<WeaponClass*>& getWeapons();
	static vector<bullet*>& getBullets();
	static vector<shell*>& getShells();
	static void checksurroundings(vector<ModelClass*> *);
	static float getWeaponPitch();
	static void setWeaponPitch(float);
	static bool getRealoadingState();
	static bool getChangingState();
};