#pragma once

#include "../networkmessage/networkmessage.h"
#include <optional>

#include "hello.h"
#include "ok.h"
#include "getstreamparams.h"
#include "streamparams.h"
#include "initializestream.h"
#include "streamframe.h"
#include "endstream.h"

namespace StudentSync::Common::Messages {
	template <typename TMessage>
	static inline std::optional<TMessage> TryReceive(TCPSocket& socket) {
		const std::optional<NetworkMessage> netMessage = NetworkMessage::TryReceive(socket);
		if (!netMessage) {
			return std::nullopt;
		}

		return TMessage::FromNetworkMessage(*netMessage);
	}
}