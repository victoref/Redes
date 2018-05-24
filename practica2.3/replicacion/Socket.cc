#include "Socket.h"
#include "Serializable.h"

#include <string.h>

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Implementación Socket
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

bool operator== (const Socket &s1, const Socket &s2)
{

	if (s1.sa.sa_family == AF_INET && s2.sa.sa_family == AF_INET)
	 {
	     struct sockaddr_in *sock_in1 = (struct sockaddr_in *) s1.sa;
	     struct sockaddr_in *sock_in2 = (struct sockaddr_in *) s2.sa;

	     return (sock_in1->sin_family == sock_in2->sin_family && sock_in1->sin_port == sock_in2->sin_port
	    		 && sock_in1->sin_addr.s_addr == sock_in2->sin_addr.s_addr);
	 }


}

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;
}

// ----------------------------------------------------------------------------

Socket::Socket(const char * address, const char * port):sd(-1)
{
	struct addrinfo hints;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	memset((void*)&hints, '\0', sizeof(struct addrinfo));

	struct addrinfo *res;

	int addrinfo = getaddrinfo(address, port, &hints, &res);

	if(addrinfo != 0){

		throw new std::runtime_error("Error getaddrinfo");
	}

	sd = socket(res->ai_family, res->ai_socktype,0);

	if(sd == -1){
		throw std::runtime_error("error al crear el socket udp");
	}

	sa = res->ai_addr;
	sa_len = res->ai_addrlen;
	
}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::send(Serializable * obj, Socket * sock)
{
	obj->to_bin();
	int aux = sendto(sock->sd,(void*)obj->_data,obj->_size,0,&sock->sa,sock->sa_len);

	if(aux == -1)
		return -1;
	

	return 0;
}

// ----------------------------------------------------------------------------

int Socket::recv(char * buffer, Socket ** sock)
{
	struct sockaddr src;
	socklen_t port = sizeof(src);

	buffer = (char*)malloc(MAX_MESSAGE_SIZE);

	char host[NI_MAXHOST];
	char server[NI_MAXSERV];

	if(sock != 0){

		ssize_t recived =  recvfrom(sd, (void* )&buffer,
		    			MAX_MESSAGE_SIZE, 0, &src, &port);
		sock = new Socket(&src,port);
		//Serializable::from_bin(buffer)
		
		if(recived == -1)
			return -1;
		
	}

	return 0;


	/*
		ssize_t recived =  recvfrom(sd, (void* )&buffer,
		    			MAX_MESSAGE_SIZE, 0, &src, &port);
		if(recived != 0){
			sock = new Socket(&src,port);
			return 0;
		}
		else
			return -1;


	*/


}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

