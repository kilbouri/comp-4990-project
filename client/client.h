#pragma once

#include <iostream>

#include "../win32includes.h"
#include "../common/socket/socket.h"
#include "../common/displaycapturer/displaycapturer.h"


class Client {
public:
	Client();

	bool Connect(std::string_view hostname, int portNumber);
	bool Disconnect();

	bool SendString(const std::string& str);
	bool SendNumber(int64_t number);
	bool SendScreenshot(const DisplayCapturer::Format format);

	~Client();
private:
	TCPSocket socket;
};