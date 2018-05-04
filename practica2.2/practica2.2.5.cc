

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <time.h>


//Ejercicio5
 int main(int argc, char* argv[]){


	struct addrinfo hints;
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo * res;

	int err = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(err != 0){
		std::cout << "Error" << std::endl;
		return -1;
	}
	

	int sd = socket(res->ai_family, res->ai_socktype, 0);
	
	if(sd == -1){
		std::cout << "Error" <<std::endl;
		return -1;
	}


	int conc = connect(sd, res->ai_addr, res->ai_addrlen);
	
	if(conc == -1){
		std::cout << "Error" <<std::endl;
		return -1;
	}
		
	
	char peticion[256];
	memset((void*) peticion, '\0',256);
	
	bool conec = true;

	struct sockaddr src_addr;
	socklen_t addrlen = sizeof(src_addr);
	
	
		
	while(conec){
		
		
		std::cin>>peticion;
		
		if(peticion[0] == 'q'){
			close(sd);
			conec = false;
			return 0;
		}
		
		send(sd,peticion,256,0);
				
		ssize_t s = recv(sd, peticion, 256, 0);
					
		std::cout << peticion << std::endl;
	}
	freeaddrinfo(res);


	return 0;
}
