

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

//Ejercicio1

int main(int argc, char* argv[]){


	struct addrinfo hints;
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;

	struct addrinfo * res;

	int err = getaddrinfo(argv[1], NULL, &hints, &res);

	if(err != 0){
		std::cout << "Error: " << gai_strerror(err)<<std::endl;
		return -1;
	}


	for(struct addrinfo* i = res; i != 0; i = i ->ai_next){
		char host [NI_MAXHOST];
		char serv [NI_MAXSERV];

		getnameinfo (i->ai_addr, i->ai_addrlen, host, NI_MAXHOST,serv, NI_MAXSERV, NI_NUMERICHOST);

		std::cout << host << " " << i->ai_family << " " << i->ai_socktype <<std::endl;
	}

	freeaddrinfo(res);


	return 0;
}

