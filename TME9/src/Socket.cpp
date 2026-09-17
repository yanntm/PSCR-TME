#include "Socket.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

namespace pr {


void Socket::close() {
	if (fd != -1) {
		shutdown(fd,1);
		::close(fd);
	}
}

void Socket::connect(const std::string & host, int port) {
	// but : resoudre l'addresse "humains" vers une addresse format machine

	struct addrinfo * addr;
	/* Remplir la structure	dest */
	// les choses à null ne servent pas ici
	if (getaddrinfo(host.c_str(), /* service*/ nullptr, /* hints*/ nullptr, &addr) != 0) {
		perror("getaddrinfo");
		return;
	}
	// champ "data" ai_addr (une addresse en reponse) du premier chainon de addrinfo
	// dedans on trouve la chose utile : une addresse de socket, qu'il faut encore
	// fixer vers un pointeur d'adresse de socket du domaine AF_INET
	// on doit faire un (down)cast pour retyper sockaddr en sockaddr_in
	in_addr ipv4 = ((struct sockaddr_in *) addr->ai_addr)->sin_addr;
	// getaddrinfo alloue une liste, on desalloue poliment
	freeaddrinfo(addr);
	connect(ipv4, port);
}

void Socket::connect(in_addr ipv4, int port) {
	// une addresse spécialisée pour internet
	sockaddr_in dest;
	dest.sin_family = AF_INET; // discriminant, aide à retrouver le type concret dans un switch case
	dest.sin_addr = ipv4;
	dest.sin_port = htons(port); // host to network

	// create socket
	fd = socket(AF_INET,SOCK_STREAM, /* protocole TCP par defaut sur SOCK_STREAM*/ 0);
	if (fd <0) {
		perror("socket");
		return;
	}

	// ici on doit upcast de sockaddr_in (spécialisé) vers sockaddr plus général
	// en OO, pas de syntaxe, en C il faut un cast
	// on passe la taille réelle de l'objet sockaddr spécialisé
	if (::connect(fd,(struct sockaddr *) &dest, sizeof dest) <0) {
		perror("connect");
		::close(fd);
		fd=-1;
		return;
	}
}

// sockaddr_in est présent dans l'API a plein d'endroits, e.g. expediteur dans un accept
// objectif : de l'addresse format machine vers du lisible humain
std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr) {
	char hname [1024];
	// obtient à partir de l'addresse machine le nom d'hote
	if (getnameinfo((struct sockaddr *)addr,sizeof *addr, hname, 1024, nullptr, 0, 0) == 0) {
		os << '"' << hname << '"' << " ";
	}
	// inet_ntoa : produit la chain "127.0.0.1" à partir d'une addresse ipv4 sur 4 octets
	// ntohs : network to host
	os << inet_ntoa(addr->sin_addr) << ":" << ntohs(addr->sin_port) << std::endl;
	return os;
}


}
