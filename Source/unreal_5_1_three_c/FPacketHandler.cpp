#include "FPacketHandler.h"

#include "FPacketHeader.h"

FPacketHandler::FPacketHandler()
{
}

FPacketHandler::~FPacketHandler()
{
}

uint8* FPacketHandler::MakePacket(P_LOGIN_PAKCET& pkt, FBufferArchive& Ar)
{
	uint16 type = 1;
	uint16 size = sizeof(FPacketHeader) + pkt.Size();
	FPacketHeader header(type, size);
	
	Ar << header.type;
	Ar << header.size;
	Ar << pkt.Type;
	Ar << pkt.IdLen;
	Ar.Append((uint8*)pkt.Id.GetCharArray().GetData(), pkt.IdLen * sizeof(TCHAR));
	return Ar.GetData();
}

uint8* FPacketHandler::MakePacket(P_MOVE_PACKET& pkt, FBufferArchive& Ar)
{
	uint16 type = 3;
	uint16 size = sizeof(FPacketHeader) + pkt.Size();
	FPacketHeader header(type, size);

	Ar << header.type;
	Ar << header.size;
	Ar << pkt.X;
	Ar << pkt.Y;
	Ar << pkt.Z;
	Ar << pkt.Yew;

	return Ar.GetData();
}

uint8* FPacketHandler::MakePacket(P_CHAT_PACKET& pkt, FBufferArchive& Ar)
{
	uint16 type = 4;
	uint16 size = sizeof(FPacketHeader) + pkt.Size();
	FPacketHeader header(type, size);

	Ar << header.type;
	Ar << header.size;
	Ar << pkt.Type;
	Ar << pkt.TextLen;
	Ar.Append((uint8*)pkt.Text.GetCharArray().GetData(), pkt.TextLen * sizeof(TCHAR));

	return Ar.GetData();
}

uint8* FPacketHandler::MakePacket(P_SKILL_PACKET& pkt, FBufferArchive& Ar)
{
	uint16 type = 7;
	uint16 size = sizeof(FPacketHeader) + pkt.Size();
	FPacketHeader header(type, size);

	Ar << header.type;
	Ar << header.size;
	Ar << pkt.SkillCode;

	return Ar.GetData();
}

uint8* FPacketHandler::MakePacket(P_HIT_PACKET& pkt, FBufferArchive& Ar)
{
	uint16 type = 8;
	uint16 size = sizeof(FPacketHeader) + pkt.Size();
	FPacketHeader header(type, size);

	Ar << header.type;
	Ar << header.size;
	Ar << pkt.TargetCode;
	Ar << pkt.AttackCode;
	
	return Ar.GetData();
}
