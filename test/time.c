// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNTING_LOOF 100

void doing() 
{
	Sleep(100);
}

double estimate(clock_t set, clock_t stop)
{
	return (double)(((double)stop - (double)set) / CLOCKS_PER_SEC);
}


int main()
{
	clock_t set = 0;
	clock_t stop = 0;

	for (;;)
	{
		set = clock();

		doing();

		stop = clock();

		printf("second for one loop : %lf s \n",estimate(set,stop));
	}




    return 0;
}

