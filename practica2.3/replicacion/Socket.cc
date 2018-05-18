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

	getaddrinfo(address, port, &hints, &res);
	sd = socket(hints.ai_family, hints.ai_socktype,0);

	if(sd == -1)
		throw std::runtime_error("error al crear el socket udp");
}

// ----------------------------------------------------------------------------

int Socket::bind()
{
    return ::bind(sd, (const struct sockaddr *) &sa, sa_len);
}

// ----------------------------------------------------------------------------

int Socket::send(Serializable * obj, Socket * sock)
{
}

// ----------------------------------------------------------------------------

int Socket::recv(char * buffer, Socket ** sock)
{
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

