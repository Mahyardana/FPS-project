#pragma once
#include <Windows.h>
using namespace std;

class Timer
{
private:
	bool slowmotion = false;
	long lastframe;
	long long difference;
	int fps;
	Timer();
	static Timer* getInstance();
	static Timer* Instance;
public:
	static void toggleSlowMotion();
	static int getDifferencems();
	static void UpdateTimer();
	static int getFPS();
	static double getDifference();
	static double getSleepTime();
	static bool getSlowMotionState();
};