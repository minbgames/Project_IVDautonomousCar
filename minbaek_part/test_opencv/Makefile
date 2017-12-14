test:
	rm test_exe
	g++ -o test_exe test_data.cpp `pkg-config opencv --cflags --libs` -std=c++11
	./test_exe

test_make:
	g++ -o test_exe test_data.cpp `pkg-config opencv --cflags --libs` -std=c++11
