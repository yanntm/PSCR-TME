#pragma once

#include "Socket.h"

namespace pr {

class ServerSocket {
	int socketfd;

public :
	// Demarre l'ecoute sur le port donne
	ServerSocket(int port);

	int getFD() { return socketfd;}
	bool isOpen() const {return socketfd != -1;}

	Socket accept();

	void close();
};

} // ns pr
