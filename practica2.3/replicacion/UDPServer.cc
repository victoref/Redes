#include "UDPServer.h"
#include "Serializable.h"

UDPServer::UDPServer(const char * serv, const char * port)
    :socket(serv, port)
{
    pthread_mutex_init(&mutex, 0);
};

UDPServer::~UDPServer()
{
    pthread_mutex_destroy(&mutex);
};

// ----------------------------------------------------------------------------

extern "C" void * _server_thread(void *arg)
{
    UDPServer * server = static_cast<UDPServer *>(arg);

    server->server_thread();

    return 0;
}

// ----------------------------------------------------------------------------

int UDPServer::start()
{

	pthread_t pThreads[THREAD_POOL_SIZE];

	for(int i = 0; i < THREAD_POOL_SIZE; i++){

		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

		int pthreadError = pthread_create(&pThreads[i], &attr, _server_thread, static_cast<void*>(this));

		if(pthreadError != 0){
			
			std::cout << "Error al crear thread" << std::endl;
			return -1;
		}

	}

	return 0;

}

// ----------------------------------------------------------------------------

void UDPServer::server_thread()
{

	while(true){

		char *buffer;
		struct sockaddr src;
		socklen_t tam = sizeof(src);

		Socket * socket2;
		socket.recv(buffer,&socket2);
		add_connection(socket2);
		do_message(buffer);
	}
}

// ----------------------------------------------------------------------------

void UDPServer::add_connection (Socket * s)
{

	pthread_mutex_lock(&mutex);

	bool exists = false;

	for(Socket* s2 : connections){

		if(s == s2){

			delete s; 
			exists = true;
		}

	}

	if(!exists && connections.size() < THREAD_POOL_SIZE)
		connections.push_back(s);
	

	pthread_mutex_unlock(&mutex);

}

// ----------------------------------------------------------------------------

void UDPServer::del_connection (Socket * s)
{

	pthread_mutex_lock(&mutex);

	bool encontrado = false;

	std::vector<Socket *>::iterator it = connections.begin();

	while (it != connections.end() && !encontrado){
		
		if((*it) == s){
			connections.erase(it);
			encontrado = true;
		}
	}

	if(!encontrado)
		delete s;

	pthread_mutex_unlock(&mutex);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

