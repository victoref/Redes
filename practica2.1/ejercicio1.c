#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <time.h>

int main (){

	int error = setuid(0);
	char err[256];
	char * errType;
	if(error == -1){
		perror("Err: ");
		errType = strerror_r(errno, err,256);
		std::cout << err << errType << std::endl;
	}
	return 0;

}
