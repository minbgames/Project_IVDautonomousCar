#include <thread>
#include <chrono>
#include <mutex>
#include <iostream>

namespace chro = std::chrono;
namespace tthr = std::this_thread;

std::mutex mtx;

int handler1(int num)
{
	mtx.lock();
	for(int i=0; i<5; i++) {
		std::cout << num << std::endl;
		tthr::sleep_for(chro::milliseconds(500));
	}
	mtx.unlock();
	return 0;
}

int main()
{
	std::thread th(handler1, 10);
	std::thread th2(handler1, 20);
	int j;

	th.join();
	th2.join();

	return 0;
}
