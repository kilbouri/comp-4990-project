#pragma once

#include <optional>

#include "../tlvmessage/tlvmessage.hpp"
#include "../socket/tcpsocket.hpp"

#include "hello.hpp"
#include "ok.hpp"
#include "getstreamparams.hpp"
#include "streamparams.hpp"
#include "initializestream.hpp"
#include "streamframe.hpp"
#include "endstream.hpp"

namespace StudentSync::Networking::Message {
	template <typename TMessage>
	static inline std::optional<TMessage> TryReceive(TCPSocket& socket) {
		const std::optional<TLVMessage> netMessage = TLVMessage::TryReceive(socket);
		if (!netMessage) {
			return std::nullopt;
		}

		return TMessage::FromNetworkMessage(*netMessage);
	}
}