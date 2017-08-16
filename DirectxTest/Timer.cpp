#include "Timer.h"
Timer* Timer::Instance = 0;


Timer::Timer()
{

}

Timer * Timer::getInstance()
{
	if (Instance)
	{
		return Instance;
	}
	else
	{
		Instance = new Timer;
		Instance->lastframe = GetTickCount64();
		Instance->difference = 0;
		return Instance;
	}
}

void Timer::toggleSlowMotion()
{
	Timer* instance = getInstance();
	instance->slowmotion = !instance->slowmotion;
}

int Timer::getDifferencems()
{
	Timer* instance = getInstance();
	int timems;
	if (instance->slowmotion)
	{
		timems = instance->difference / 10;
	}
	else
	{
		timems = instance->difference;
	}
	return timems;
}

void Timer::UpdateTimer()
{
	Timer* instance = getInstance();
	instance->difference = GetTickCount64() - instance->lastframe;
	if (instance->difference > 0)
		instance->fps = 1000 / instance->difference;
	else
		instance->fps = 0;
	instance->lastframe = GetTickCount64();
}

int Timer::getFPS()
{
	Timer* instance = getInstance();
	return instance->fps;
}

double Timer::getDifference()
{
	Timer* instance = getInstance();
	double differ;
	if (instance->slowmotion)
	{
		differ = (double)instance->difference / 10000;
	}
	else
	{
		differ = (double)instance->difference / 1000;
	}
	return differ;
}

double Timer::getSleepTime()
{
	Timer* instance = Timer::getInstance();
	return instance->difference;
}

bool Timer::getSlowMotionState()
{
	Timer* instance = Timer::getInstance();
	return instance->slowmotion;
}
