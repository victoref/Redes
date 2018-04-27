

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

//Ejercicio3

 int main(int argc, char* argv[]){


	struct addrinfo hints;
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo * res;

	int err = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(err != 0){
		std::cout << "Error: " << gai_strerror(err)<<std::endl;
		return -1;
	}


	char host [NI_MAXHOST];
	char serv [NI_MAXSERV];

	getnameinfo (res->ai_addr, res->ai_addrlen, host, NI_MAXHOST,serv, NI_MAXSERV, NI_NUMERICHOST);
	std::cout << host << " " << res->ai_family << " " << res->ai_socktype <<std::endl;

	int sd = socket(res->ai_family, res->ai_socktype, 0);
	
	if(sd == -1){
		std::cout << "Error: " << gai_strerror(err)<<std::endl;
		return -1;
	}

	char peticion[20];
	memset((void*) peticion, '\0',20);
	peticion[0] = argv[3][0];

	sendto(sd,peticion,20,0,res->ai_addr,res->ai_addrlen);
	
	if(peticion[0] == 'q'){
		return 0;
	}
	struct sockaddr src_addr;
	socklen_t addrlen = sizeof(src_addr);
		
	recvfrom(sd, peticion, 20, 0, &src_addr, &addrlen);
	
	std::cout << peticion << std::endl;

	freeaddrinfo(res);


	return 0;
}

