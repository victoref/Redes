#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <math.h>

int main (){

	struct timespec tIni, tFin;
	int var = 0;
	
	clock_gettime(CLOCK_REALTIME,&tIni);
	for(int i= 0; i < 10000; i++){
		var++;
	}
	clock_gettime(CLOCK_REALTIME, &tFin);
	
	time_t t = tFin.tv_nsec - tIni.tv_nsec;
	long l = pow((tFin.tv_nsec - tIni.tv_nsec), 2);
	std::cout<<t<<std::endl;
	std::cout<<l<<std::endl;
	return 1;
}
