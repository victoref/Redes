

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <time.h>


//Ejercicio4

 int main(int argc, char* argv[]){


	struct addrinfo hints;
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

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
		std::cout << "Error: " << gai_strerror(sd)<<std::endl;
		return -1;
	}

	int aux = bind(sd, res->ai_addr, res->ai_addrlen);

	
	if(aux == -1){
		std::cout << "Error: " << gai_strerror(aux)<<std::endl;
		return -1;
	}
	
	listen(sd,15);
	bool conec = true;
	/////admite todas las conexiones posibles
	while(true){
		struct sockaddr src_addr;
		socklen_t addrlen = sizeof(src_addr);
		
		int sd_src = accept(sd, &src_addr, &addrlen);
		
		conec = true;

		while(conec){
			
			char buf [256];

			ssize_t s = recv(sd_src, &buf, 255, 0);
			
			if(s == 0){ // Se ha cerrado la conexión.
				conec =false;
				return 0;
			}
			

			getnameinfo(&src_addr, addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST);
			std::cout << host << " " << serv <<std::endl;

			
			send(sd_src,buf, s, 0);


		}
	}

	freeaddrinfo(res);


	return 0;
}
