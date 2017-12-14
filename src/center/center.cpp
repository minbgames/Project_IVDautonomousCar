#include <mutex>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>

#include "LidarManager.hpp"
#include "Serial.hpp"
#include "Control.hpp"
#include "../common/UDSClient.hpp"
#include "PID.hpp"

namespace chro = std::chrono;
namespace tthr = std::this_thread;

bool pidPrintFlag = false;

int cmdHandler();
int lidarHandler();

LidarManager l;
Control control("/dev/ttyUSB0");
PID pid;

int mode = 0;
//0 : Lane tracking
//1 : Stop
//2 : Narrow, S
//3 : Parking

int main()
{
	//l.setUpLidar();
	printf("lidar set up complete!\n");
	//l.start();
	printf("lidar start!\n");

	UDSClient client("/tmp/gtserver");
	client.start();

	//PCU와 시리얼 연결 설정

	control.start();

	bool loopFlag = true;


	std::thread cmdThread(cmdHandler);
	//std::thread lidarThread(lidarHandler);

	std::cout << "Central process on" << std::endl;

	control.setMode(Mode::AUTO);
	//control.setSpeed(25);
	float temp;
	float stopLine = 999;
	float parkLine = 999;
	float signInput;
	int tempPID;
	for(;loopFlag;) {
		if(mode == 0) {
			temp = client.recvFloat();
			//stopLine = client.recvFloat();
			//parkLine = client.recvFloat();
			if(parkLine < 20) {
				//control.addAction2(Gear::FORWARD, 0, 20, 50);
				//control.addAction(Gear::FORWARD, 0, 0, 40);
			}
			//signInput = client.recvFloat();
			//client.sendInt(0); //0:steer, 1:inputsign
			//client.sendInt(control.getSteer());
		}
		else if(mode == 2) {
			temp = (float)l._gap/3.0f;
		}

		//tthr::sleep_for(chro::milliseconds(10));
		if(control.getMode() == Mode::AUTO) {
			if(mode == 0) {
				tempPID = pid.process(temp);
			}
			else if(mode == 2) {
				tempPID = temp*3.3;
			}
		}
		else {
			pid.init();
			if(!control.isBusy()) {
				control.setSteer(0);
			}
		}
		if(pidPrintFlag) {
			std::cout <<std::setw(10) << temp;
			std::cout <<std::setw(10) << tempPID << std::endl;
		}

		if(!control.isBusy()) {
			if(tempPID > 2000) tempPID = 2000;
			if(tempPID < -2000) tempPID = -2000;
			std::cout << -tempPID << std::endl;
			control.setSteer(-tempPID);
		}

	}


	control.end();
	control.stopThread();
	cmdThread.join();

	std::cout << "Exit success" << std::endl;
	return 0;
}


int cmdHandler()
{
	char command;
	int input;
	while(1) {
		command = getchar();

		switch(command) {
			//프로그램 종료
			case 'q':
				exit(0);
				break;
			//PCU제어 테스트
			case 's':
				std::cout << "input steer : ";
				std::cin >> input;
				control.setSteer(input);
				break;
			case 'p':
				std::cout << "input power : ";
				std::cin >> input;
				control.setSpeed(input);
				break;
			case '1':
				control.addAction2(Gear::FORWARD, 0, 25, 200);
				control.addAction(Gear::FORWARD, 0, 0, 220);
				control.addAction(Gear::FORWARD, 0, 25, 100);
				break;
			case '2':
				control.addAction2(Gear::FORWARD, -2000, 25, 600);
				control.addAction(Gear::FORWARD, 0, 25, 100);
				break;
			case 'f':
				std::cout << "1:state 2:pidprint ";
				std::cin >> input;
				switch(input) {
				case 1:
					control.printFlag = !control.printFlag;
					break;
				case 2:
					pidPrintFlag = !pidPrintFlag;
					break;
				}
				break;
			case 'c':
				std::cout << "1:Kp 2:Ki 3:Kd 4:coef 5:cycletime 6:alph ";
				std::cin >> input;
				float ivalue;
				switch(input) {
				case 1:
					std::cout << "current : " << pid.Kp  << std::endl;
					std::cout << "input Kp" << std::endl;
					std::cin >> ivalue;
					pid.Kp = ivalue;
					break;
				case 2:
					std::cout << "current : " << pid.Ki  << std::endl;
					std::cout << "input Ki" << std::endl;
					std::cin >> ivalue;
					pid.Ki = ivalue;
					break;
				case 3:
					std::cout << "current : " << pid.Kd  << std::endl;
					std::cout << "input Kd" << std::endl;
					std::cin >> ivalue;
					pid.Kd = ivalue;
					break;
				case 4:
					std::cout << "current : " << pid.coef  << std::endl;
					std::cout << "input coef" << std::endl;
					std::cin >> ivalue;
					pid.coef = ivalue;
					break;
				case 5:
					std::cout << "current : " << pid.cycle_time  << std::endl;
					std::cout << "input cycletime" << std::endl;
					std::cin >> ivalue;
					pid.cycle_time = ivalue;
					break;
				case 6:
					std::cout << "current : " << pid.alpha  << std::endl;
					std::cout << "input alpha" << std::endl;
					std::cin >> ivalue;
					pid.alpha = ivalue;
					break;
				}
				break;

			default:
				break;
		}
	}

	return 0;
}

int lidarHandler()
{
	while(1) {
		std::cout << "sdfds " << std::endl;
		//l.run();
	}
	return 0;
}
