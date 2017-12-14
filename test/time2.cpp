#include <iostream>
#include <chrono>

int main(void)
{

  int num = 12345;

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  for(int i=0; i<100000000; i++) {
    num = num%17;
    num = 12345;
  }
  end = std::chrono::system_clock::now();

  std::chrono::duration<double> elapsed_time = end-start;
  std::cout << elapsed_time.count() << std::endl;
}
