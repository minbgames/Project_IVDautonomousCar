#include "LidarManager.h"
#include <time.h>
#include <thread>
#include <chrono>
#include <iostream>

namespace chro = std::chrono;
namespace tthr = std::this_thread;

int main(void)
{
	LidarManager l;
	l.setUpLidar();
	printf("lidar set up complete!\n");
	l.start();
	printf("lidar start!\n");

	while(1){
		l.run();
	}


	l.stop();
	return 0;
}
