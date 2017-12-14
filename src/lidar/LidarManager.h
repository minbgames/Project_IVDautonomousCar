#ifndef __LIDAR_MANAGER_H__
#define __LIDAR_MANAGER_H__


// includes
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "LMS1xx.h"


// defines

#define PI				3.14159265
#define INIT_CRITICAL_AREA  		3000

#define LIDAR_MAX_DISTANCE  		10000
#define LIDAR_DATA_SIZE     		541
#define LIDAR_RESOLUTION    		5000
#define LIDAR_FREQUENCY     		5000
#define OBJECTRECOGMAXVAL		10
#define OBJECTRECOGVAL			3

#define USEAREA01			90
#define USEAREA02			450


#define NUM_OF_AREA			27
#define AREA_SIZE			20

#define UNDEFINE			0
#define READY_FOR_MEASURE   		7

#define LOSS_DATA			-1

#define ENCOUNTER			1
#define UNENCOUNTER			0

#define LIDAR_RSSI_MIN			20

#define START				1
#define STOP				0

#define UNACT				0
#define ACT1				1
#define ACT2				2
#define ACT3				3



// define of LidarManager
class LidarManager
{

// LidarManager value
private:
  // LMS Lidar device
  LMS1xx    	_device;
  scanData  	_data;
  int		_objects[LIDAR_DATA_SIZE];
  int		_areas[AREA_SIZE];
  int       	_status;
  int		_critical;

  void setUpScanCfg();


public:
  LidarManager();
  int  _mode;
  // set up Link Lidar Deivce
  void setUpLidar();
  void run();
  void start();
  void stop();
  void setCritical(int critical);
  int  getObjectSize(double angle, int distance );
  void testData();
  void traceWithAngle(int angle);

private:
  void connect();
  void disconnect();
  void update();
  void interpolate();
  void trace();
};



#endif
