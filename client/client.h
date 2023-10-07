#pragma once

#include <iostream>

#include "../win32includes.h"

class Client {
public:
	Client(std::string_view serverHostname, int serverPort);

	int Initialize();
	int Connect();

	~Client();
private:
	std::string hostname;
	std::string port;

	struct addrinfo hints;
	struct addrinfo* serverAddress;
	SOCKET connectSocket;
};