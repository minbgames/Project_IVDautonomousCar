#include "LidarManager.h"

LidarManager::LidarManager()
{
	_mode		= ACT1;
	_status		= UNDEFINE;
	setCritical(INIT_CRITICAL_AREA); // 3m
	memset(_objects, 0, sizeof(_objects));
	memset(_areas, 0, sizeof(_areas));
}

/*
*/
void LidarManager::setUpScanCfg()
{
	scanCfg c = _device.getScanCfg();

	c.angleResolution = LIDAR_RESOLUTION;
	c.scaningFrequency = LIDAR_FREQUENCY;

	_device.setScanCfg(c);

	scanDataCfg cc;
	cc.deviceName = false;
	cc.encoder = 0;
	cc.outputChannel = 3;
	cc.remission = true;
	cc.resolution = 0;
	cc.position = false;
	cc.outputInterval = 1;

	_device.setScanDataCfg(cc);
	//_device.startDevice();
}

/*
*/

void LidarManager::setUpLidar()
{
	connect();

	if( _device.isConnected() == false )
	{
		std::cout << "Connection Fail!" << std::endl;
		exit(1);
	}
	_device.login();
	_device.stopMeas();
	setUpScanCfg();

}

void LidarManager::connect()
{
	_device.connect("192.168.0.1");
}

void LidarManager::disconnect()
{
	_device.disconnect();
}

void LidarManager::run()
{
	update();
}

/*


*/

int  LidarManager::getObjectSize(double angle, int distance)
{
	return 2 * (int)((double)distance * tan((double)angle/2*(PI/180)));
}

void LidarManager::interpolate()
{
	for (int i = USEAREA01; i < USEAREA02 ; i++)
	{
		if (_data.dist1[i] > LIDAR_MAX_DISTANCE )
			_data.dist1[i] = LIDAR_MAX_DISTANCE;

		if (_data.rssi1[i] < LIDAR_RSSI_MIN)
			_data.dist1[i] = LOSS_DATA;

		// write object Data sheets...
		if (_data.dist1[i] <= _critical && _data.dist1[i] != LOSS_DATA)
			_objects[i] = (_objects[i] + 1) % OBJECTRECOGMAXVAL;
		else
			_objects[i] = _objects[i] / 2;
	}
}

void LidarManager::trace()
{
	int object_start = 0;
	int object_end = 0;
	int object_length = 0;
	int object_count = 0;
	int arr_areaStart[100];
	int arr_areaEnd[100];
	int arr_distance[100];
	int arr_length[100];

	for (int i = USEAREA01; i < USEAREA02; i++)
	{
		int min_distance = 999999;
		if(_objects[i] > OBJECTRECOGVAL){


			object_start = i;

			while(_objects[i] > OBJECTRECOGVAL)
			{
				min_distance = min_distance < _data.dist1[i] ? min_distance : _data.dist1[i];
				i++;
				if(_objects[i] <= OBJECTRECOGVAL)
				{
					int __start = i;
					int __end = i + 30;
					for (__start; __start < __end ; __start++)
					{
						if( _objects[__start] > OBJECTRECOGVAL ){
							i = __start;
							break;
						}
					}
				}
			}

			object_end = i;
			if(min_distance > _critical) min_distance = _critical;
			object_length = getObjectSize((double)(object_end - object_start)/2 , min_distance);

			printf("{%ld} Area : [ %d  -- %d ], Distance : [ %d ], Length of Object : [ %d ] \n ", time(NULL), object_start, object_end, min_distance, object_length);

			arr_areaStart[object_count] = object_start;
			arr_areaEnd[object_count] = object_end;
			arr_distance[object_count] = min_distance;
			arr_length[object_count] = object_length;

			object_count++;


			fflush(stdout);
		}
	}

	printf(" Count of Object : %d \n", object_count);

	if(object_count == 0) printf("No Objects!\n");
}


void LidarManager::traceWithAngle(int angle)
{
	int angle1 = 0;
	int angle2 = 0;

	if(_objects[270 - angle*2] > OBJECTRECOGVAL)
	{
		printf(" [ 270 - THETA : Distance : %d] \n", _data.dist1[270 - angle*2]);
		angle1 = 1;
	}
	if(_objects[270 + angle*2] > OBJECTRECOGVAL)
	{
	 	printf(" [ 270 + THETA : Distance : %d] \n", _data.dist1[270 + angle*2]);
		angle2 = 1;
	}

	if(angle1 == 1 && angle2 == 1)
	{
		printf("gap : %d \n" , _data.dist1[270 + angle*2] - _data.dist1[270 - angle*2]);
	}
}
void LidarManager::update()
{
	_device.getData(_data);
	interpolate();

	switch(_mode)
	{
		case ACT1 :
			trace();
			break;
		case ACT2 :
			traceWithAngle(30);
			break;
	}
}

void LidarManager::testData()
{
	_device.getData(_data);
	for (int i = USEAREA01; i < USEAREA02 ; i++)
		printf("%d ", _data.dist1[i]);
}

void LidarManager::start()
{
	_device.startMeas();

	/*
	while (_status != READY_FOR_MEASURE)
	{
		_status = _device.queryStatus();
		std::cout << "status : " << _status << std::endl;
		sleep(1);
	}*/
	_device.startDevice();
	_device.scanContinous(START);
	printf("Start Scan...\n");
}

void LidarManager::stop()
{
	_device.scanContinous(STOP);
	_device.stopMeas();
}

void LidarManager::setCritical(int critical)
{
	_critical = critical;
}
