#include "ServerSocket.h"
#include <cstring>
#include <unistd.h>
#include <iostream>

namespace pr {

ServerSocket::ServerSocket(int port) :socketfd(-1) {

	// create socket
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if (fd == -1) {
		perror("create socket");
		return;
	}

	// bind
	struct sockaddr_in sin;  /* Nom de la socket de connexion */

	memset(&sin, 0, sizeof(sin)); // utile ?
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY); // on attend sur n'importe quelle interface de la machine
	sin.sin_port = htons(port); // host to network

	/* nommage, meme probleme de typage sockaddr que dans la Socket */
	if (bind(fd, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
		perror("bind");
		// on veut le close de unistd, pas le close membre de cette classe
		::close(fd);
		return;
	}

	// listen
	if (listen(fd, 50) < 0) {
		perror("listen");
		::close(fd);
		return;
	}

	// success !
	socketfd = fd;
}

Socket ServerSocket::accept() {
	struct sockaddr_in exp;
	socklen_t len = sizeof(exp);
	// on qualifie sinon le compilo rale
	int scom = ::accept(socketfd, (struct sockaddr *) &exp, &len);
	if (scom < 0) {
		perror("accept");
	} else {
		// en appui sur operator << développé plus haut
		std::cout << "Accepted connection from " << &exp << std::endl;
	}
	return scom;
}

void ServerSocket::close() {
	if (socketfd != -1) {
		::close(socketfd);
	}
}

}
