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
	// Ar << pkt.PwLen;
	Ar.Append((uint8*)pkt.Id.GetCharArray().GetData(), pkt.IdLen * sizeof(TCHAR));
	// Ar.Append((uint8*)pkt.Pw.GetCharArray().GetData(), pkt.PwLen * sizeof(TCHAR));
	// 넌 진짜 병신이다 .
	// 와 함수안에 객체 생성해두고
	// 그객체ㅔ 주소값을 넘기 냐 ㅋㅋㅋㅋ
	// ㅅㅂ 몇시간을 날렸지 십비ㅏㅇ라ㅏ아라
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
	
	// // FBufferArchive 생성
	// FBufferArchive Buffer;
	//
	// // FString(한글 문자열)을 추가
	// FString KoreanString = pkt.Text;
	// Buffer.Append(KoreanString);
	//
	// // Buffer의 길이 얻기
	// int32 BufferLength = Buffer.Num();
	// int32 BufferLength2 = FTCHARToUTF8(*KoreanString).Length();
	// UE_LOG(LogTemp, Warning, TEXT("BufferLength !!! %d %d"), BufferLength, BufferLength2);

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
