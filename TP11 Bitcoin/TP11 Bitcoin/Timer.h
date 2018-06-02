#pragma once
#include <chrono>


#define TIMERERROR (-1)

class Timer
{
public:
	Timer();
	Timer(float waitTime_) : waitTime(waitTime_) {}
	~Timer();
	void start();
	void stop();
	float getTime(); // In ms
	void wait(float ms) { start(); do { stop(); } while (getTime() < ms); }
	void wait() { start(); do { stop(); } while (getTime() < this->waitTime); }
	void setWaitTime(float waitTime_) { this->waitTime = waitTime_; }
private:
	std::chrono::high_resolution_clock::time_point  startValue;
	bool getStartValue;
	std::chrono::high_resolution_clock::time_point  stopValue;
	bool getStopValue;
	std::chrono:: duration<float> timePassed;
	float waitTime = 100000;
};

