#pragma once
#include "../screenresolution/screenresolution.h"
#include "../networkmessage/networkmessage.h"

namespace StudentSync::Common::Messages {
	struct StreamParams {
		long frameRate;
		ScreenResolution resolution;

		static std::optional<StreamParams> FromNetworkMessage(const NetworkMessage& netMessage) noexcept {
			if (netMessage.tag != NetworkMessage::Tag::StreamParams) {
				return std::nullopt;
			}

			int32_t fps;
			uint32_t width, height;

			constexpr size_t requiredSize = sizeof(fps) + sizeof(width) + sizeof(height);
			if (netMessage.data.size() < requiredSize) {
				return std::nullopt;
			}

			const uint8_t* dataPtr = netMessage.data.data();

			std::memcpy(&fps, dataPtr, sizeof(fps));
			std::memcpy(&width, dataPtr += sizeof(fps), sizeof(width));
			std::memcpy(&height, dataPtr += sizeof(width), sizeof(height));

			fps = ntohl(fps);
			width = ntohl(width);
			height = ntohl(height);

			return StreamParams{
				.frameRate = fps,
				.resolution = ScreenResolution(width, height)
			};
		}

		NetworkMessage ToNetworkMessage() const noexcept {
			int32_t fps = htonl(frameRate);
			uint32_t width = htonl(resolution.width);
			uint32_t height = htonl(resolution.height);

			constexpr size_t requiredSize = sizeof(fps) + sizeof(width) + sizeof(height);

			NetworkMessage::Value value{ requiredSize, 0 };
			uint8_t* dataPtr = value.data();

			std::memcpy(dataPtr, &fps, sizeof(fps));
			std::memcpy(dataPtr += sizeof(fps), &width, sizeof(width));
			std::memcpy(dataPtr += sizeof(width), &height, sizeof(height));

			return NetworkMessage(NetworkMessage::Tag::StreamParams, value);
		}
	};
}