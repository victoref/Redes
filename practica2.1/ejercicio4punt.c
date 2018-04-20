#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <time.h>

int main (){

	time_t i;
	time(&i);
	char* tiempo = ctime(&i);
	std::cout<<tiempo<<std::endl;
	return 1;

}
