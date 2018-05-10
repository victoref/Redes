

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
	ServerThread(int s) :sd(s), conec(true) {};
	virtual~ServerThread(){ close(sd); };
	void do_msg(){
		//while
		while (conec){

			char buf[256];

			ssize_t s = recv(sd, &buf, 255, 0);

			if (s == 0){ // Se ha cerrado la conexión.
				conec = false;
			}

			else{
				
				
				send(sd, &buf, s, 0);
			}

		}
	};


private:
	int sd;
	bool conec;

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
	hints.ai_socktype = SOCK_STREAM;

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

	listen(sd, 15);

		//pthread_t tid;
	//pthread_t * tids[NUM_THREADS];
	while (true){

		struct sockaddr src_addr;
		socklen_t addrlen = sizeof(src_addr);

		int sd_src = accept(sd, &src_addr, &addrlen);

		
		pthread_t tid;
		pthread_attr_t attr;

		ServerThread * st = new ServerThread(sd_src);

		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

		pthread_create(&tid, &attr, start_routine, static_cast<void*>(st));
	}

	/*for (int i = 0; i < NUM_THREADS; i++){
		pthread_join(tids[i], NULL);
	}*/

	//hacer join
	char c;
	std::cin >> c;



	freeaddrinfo(res);


	return 0;
}
