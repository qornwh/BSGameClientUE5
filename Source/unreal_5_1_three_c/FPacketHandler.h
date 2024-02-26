#pragma once
#include "Protocol/P_PROTOCOL_PAKCET.h"
#include "Serialization/BufferArchive.h"

class AMyFSocketActor;

class FPacketHandler
{
public:
	FPacketHandler();
	~FPacketHandler();

	static uint8* MakePacket(P_LOGIN_PAKCET& pkt, FBufferArchive& Ar);
	static uint8* MakePacket(P_MOVE_PACKET& pkt, FBufferArchive& Ar);
	static uint8* MakePacket(P_CHAT_PACKET& pkt, FBufferArchive& Ar);
	static uint8* MakePacket(P_SKILL_PACKET& pkt, FBufferArchive& Ar);
	static uint8* MakePacket(P_HIT_PACKET& pkt, FBufferArchive& Ar);

private:
	enum TYPES
	{
		LOGIN = 1,
		MOVE = 2,
		ATTACK = 3,
	};
};
