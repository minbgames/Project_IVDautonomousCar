per:
	rm Perception_exe
	g++ -o Perception_exe perception.cpp `pkg-config opencv --cflags --libs` -std=c++11
	./Perception_exe

tra:
	rm Traffic_exe
	g++ -o Traffic_exe traffic.cpp `pkg-config opencv --cflags --libs` -std=c++11
	./Traffic_exe

per_make:
	g++ -o Perception_exe perception.cpp `pkg-config opencv --cflags --libs` -std=c++11

tra_make:
	g++ -o Traffic_exe traffic.cpp `pkg-config opencv --cflags --libs` -std=c++11
