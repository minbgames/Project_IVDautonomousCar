#include <functional>
#include <iostream>

void f()
{
	std::cout << "call" << std::endl;
}

int main(int argc, char *argv[])
{
	std::function<void(void)> func = nullptr;
	//func = f;
	std::cout << argc << argv[0] << std::endl;

	if(func != nullptr)
		func();
	int c;
	std::cin >> c;
	return 0;
}
