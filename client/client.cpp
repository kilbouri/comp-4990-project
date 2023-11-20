#include "client.h"

#include <fstream>
#include <optional>
#include <string>

#include "../common/socket/socket.h"
#include "../common/networkmessage/networkmessage.h"
#include "../common/messages/stringmessage.h"
#include "../common/messages/number64message.h"

Client::Client() : socket(TCPSocket{}) {}

bool Client::Connect(std::string_view hostname, int portNumber) {
	return socket.Connect(hostname, portNumber);
}

bool Client::Disconnect() {
	return socket.Close();
}

bool Client::SendString(const std::string& str) {
	return StringMessage(str).ToNetworkMessage().Send(socket);
}

bool Client::SendNumber(int64_t number) {
	return Number64Message(number).ToNetworkMessage().Send(socket);
}

bool Client::StartVideoStream() {
	// video frames are required to be in PNG format. Maybe in the future we will swap over to BMP to perform temporal compression
	std::optional<std::vector<byte>> firstFrame = DisplayCapturer::CaptureScreen(DisplayCapturer::Format::PNG);
	if (!firstFrame) {
		return false;
	}

	return NetworkMessage(NetworkMessage::Tag::StartStream, std::move(*firstFrame)).Send(socket);
}

bool Client::SendVideoFrame() {
	// video frames are required to be in PNG format. Maybe in the future we will swap over to BMP to perform temporal compression
	std::optional<std::vector<byte>> frame = DisplayCapturer::CaptureScreen(DisplayCapturer::Format::PNG);
	if (!frame) {
		return false;
	}

	return true;
	//return NetworkMessage(NetworkMessage::Tag::VideoFramePNG, std::move(*firstFrame)).Send(socket);
}

bool Client::EndVideoStream() {
	return true;
	//return NetworkMessage(NetworkMessage::Tag::EndStream).Send(socket);
}

bool Client::RequestVideoStream() {
	return true;
	//return NetworkMessage::CreateRequestVideoStream().Send(socket);
}
/*
bool Client::HandleVideoStreamResponse() {

	if (!response.TryReceive(socket)) {
		// Failed to receive response
		return false;
	}

	if (response.IsAcceptVideoStream()) {
		// Server accepted the video stream request
		return true;
	}
	else if (response.IsDenyVideoStream()) {
		// Server denied the video stream request
		return false;
	}
	else {
		// Unexpected response type
		return false;
	}
}
*/

Client::~Client() {
	socket.Close();
}