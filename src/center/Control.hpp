#pragma once

#include <string>
#include <queue>
#include "Serial.hpp"
#include "Receiver.hpp"

enum class Mode {
	AUTO, MANUAL
};

enum class Gear {
	FORWARD, NEUTRAL, BACKWARD
};

typedef struct Action {
	Gear gear;
	int steer;
	int speed;
	int dist;
}Action;


class Control : public Receiver
{
private:
	char sArr[14]; //sending packet
	char rArr[13]; //receiving packet
	std::queue<Action> actionQueue;
	Serial serial;
	int waitUpdate();

protected:
	virtual void startThread();
	virtual void inFunc();

public:

	bool printFlag = false;

	Control(std::string devPath);
	~Control();

	virtual void setHandler(void (*pf)(void));

	int start();
	int end();

	int sendCommand();

	int addAction(Gear gear, int steer, int speed, int time);
	int addAction2(Gear gear, int steer, int speed, int dist);
	bool isBusy();

	std::string toString();

	Mode getMode();
	int setMode(Mode mode);

	bool getEstop();
	int setEstop(bool estop);

	Gear getGear();
	int setGear(Gear gear);

	int getSpeed();
	int setSpeed(int speed);

	int getSteer();
	int setSteer(int steer);

	int getBraking();
	int setBraking(int braking);

	int getEncoder();

	char getAlive();

};
