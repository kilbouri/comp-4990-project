#pragma once

#include <iostream>
#include <functional>
#include <vector>

#include "../common/networkmessage/networkmessage.h"
#include "../common/task/task.h"
#include "../win32includes.h"
#include <future>

/// <summary>
/// Base class for all Server implementations. Provides basic server operations and facilities to script
/// server-client conversation.
/// </summary>
struct Server {
	/// <summary>
	/// A context object which provides methods for connection handlers to communicate with the server
	/// in a safe, extendable manner.
	/// </summary>
	struct ConnectionContext {
		/// <summary>
		/// Tells the server that the handler would like to send the provided message.
		/// </summary>
		virtual Task<void> Send(NetworkMessage message) = 0;

		/// <summary>
		/// Tells the server that the handler would like to receive another message.
		/// </summary>
		virtual Task<std::optional<NetworkMessage>> Recieve() = 0;

	protected:
		std::optional<NetworkMessage> latestMessage;
	};

	bool BindAndListen(std::string& ipAddress, int portNumber);

	void Run();
	virtual void Stop(bool now = false) = 0;
	virtual bool IsStopRequested() = 0;
	virtual int GetConnectionCount() = 0;

	std::optional<std::string> GetHostname();
	std::optional<int> GetPort();

	using ConnectionHandlerFunc = std::function<Task<void>(std::shared_ptr<ConnectionContext>)>;
	void SetConnectionHandler(ConnectionHandlerFunc handler);

	~Server();

protected:
	TCPSocket listenSocket;
	std::optional<ConnectionHandlerFunc> connectionHandlerFunc;

	virtual void DoRun() = 0;
};

struct MultiConnectServer : public Server {
	struct ConnectionContext : Server::ConnectionContext {
		Task<void> Send(NetworkMessage message) override;
		Task<std::optional<NetworkMessage>> Recieve() override;

		ConnectionContext(TCPSocket socket);

	private:
		TCPSocket clientSocket;
	};

	struct Connection {

		void Terminate();
		bool IsClosed() const;

		Connection(TCPSocket socket, ConnectionHandlerFunc& handlerFunc);
	private:
		//! due to initialization order, the socket must come before the future.
		TCPSocket socket;
		std::future<void> future;

		static std::future<void> CreateFuture(TCPSocket socket, ConnectionHandlerFunc& handler);
	};

	 // todo: optional connection limit
	MultiConnectServer();

	// Inherited via Server
	void Stop(bool now) override;
	bool IsStopRequested() override;
	int GetConnectionCount() override;

protected:
	void DoRun() override;

private:
	std::vector<Connection> connections;
};