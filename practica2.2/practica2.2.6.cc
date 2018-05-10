

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>


//Ejercicio6
//compilar -l para enlazarla con la libreria, ps-aT mirar hilos

class ServerThread{
public:
	ServerThread(int s) :sd(s){};
	virtual~ServerThread(){};
	void do_msg(){
		//while
		while (true){

			char buf[256];

			struct sockaddr src_addr;
			socklen_t addrlen = sizeof(src_addr);

			ssize_t size = recvfrom(sd, buf, 255, 0, &src_addr, &addrlen);

			getnameinfo(&src_addr, addrlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST);
			std::cout << host << " " << serv << std::endl;

			if (buf[0] == 't'){
				std::cout << size << "bytes" << std::endl;
				char tiempo[256];
				memset((void*)tiempo, '\0', 256);
				struct tm* ctime;
				time_t t;
				time(&t);
				ctime = localtime(&t);
				strftime(tiempo, 256, "%H:%M:%S\n", ctime);
				sendto(sd, tiempo, 256, 0, &src_addr, addrlen);

			}
			else if (buf[0] == 'd'){
				std::cout << size << " bytes" << std::endl;
				char tiempo[256];
				memset((void*)tiempo, '\0', 256);
				struct tm* ctime;
				time_t t;
				time(&t);
				ctime = localtime(&t);
				strftime(tiempo, 256, "%Y-%m-%d\n", ctime);
				sendto(sd, tiempo, 256, 0, &src_addr, addrlen);

			}
			else if (buf[0] == 'q'){
				std::cout << size << " bytes" << std::endl << "Saliendo..." << std::endl;
				break;
			}
			else{
				std::cout << "Comando incorrecto" << std::endl;

			}
		}
	};


private:
	int sd;
};


extern "C" void*start_routine(void* _st){
	ServerThread * st = static_cast<ServerThread*>(_st);
	st->do_msg();
	delete st;
	return 0;
}

#define NUM_THREADS 10 


 int main(int argc, char* argv[]){


	struct addrinfo hints;
	memset((void*)&hints, '\0', sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	struct addrinfo * res;

	int err = getaddrinfo(argv[1], argv[2], &hints, &res);

	if(err != 0){
		std::cout << "Error" <<std::endl;
		return -1;
	}


	char host [NI_MAXHOST];
	char serv [NI_MAXSERV];

	getnameinfo (res->ai_addr, res->ai_addrlen, host, NI_MAXHOST,serv, NI_MAXSERV, NI_NUMERICHOST);
	std::cout << host << " " << res->ai_family << " " << res->ai_socktype <<std::endl;

	int sd = socket(res->ai_family, res->ai_socktype, 0);
	
	if(sd == -1){
		std::cout << "Error" <<std::endl;
		return -1;
	}

	int aux = bind(sd, res->ai_addr, res->ai_addrlen);

	
	if(aux == -1){
		std::cout << "Error" << std::endl;
		return -1;
	}

		//pthread_t tid;
	pthread_t * tids[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++){

		pthread_attr_t attr;

		ServerThread * st = new ServerThread(sd);

		pthread_attr_init(&attr);
		pthread_attr_setdetachedstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_create(&tid, &attr, start_routine, static_cast<void*>(st));
	}

	//join hilos
	for (int i = 0; i < NUM_THREADS; i++){
		pthread_join(tids[i], NULL);
	}



	freeaddrinfo(res);


	return 0;
}
