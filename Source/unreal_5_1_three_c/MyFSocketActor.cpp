// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFSocketActor.h"

#include <string>

#include "FPacketHeader.h"
#include "MyGameInstance.h"
#include "unreal_5_1_three_cGameMode.h"
#include "Common/TcpSocketBuilder.h"
#include "Struct/PlayerInfo.h"

// Sets default values
AMyFSocketActor::AMyFSocketActor()
{
	PrimaryActorTick.bCanEverTick = true;
	// PrimaryActorTick.TickInterval = 0.4f;
}

AMyFSocketActor::AMyFSocketActor(FSocket* ClientSocket) : _clientSocket(ClientSocket)
{
	PrimaryActorTick.bCanEverTick = true;
}

AMyFSocketActor::~AMyFSocketActor()
{
	if (_clientSocket)
	{
		this->_clientSocket->Close();
		UE_LOG(LogTemp, Warning, TEXT("~AMYSocket Destroyed!!!"));
	}
}

// Called when the game starts or when spawned
void AMyFSocketActor::BeginPlay()
{
	Super::BeginPlay();

	// if (_clientSocket)
	// {
	// 	// 데이터 송신 후 처리
	// 	IsConnect.Store(true);
	// 	UE_LOG(LogTemp, Warning, TEXT("Socket Connected"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Socket Connected Failed"));
	// }

	__readBuffer = new char[4096];
	__sendBuffer = new char[4096];
}

void AMyFSocketActor::Destroyed()
{
	Super::Destroyed();

	if (_clientSocket)
	{
		if (IsConnect && _clientSocket->GetConnectionState() == SCS_Connected)
		{
			_clientSocket->Close();
			UE_LOG(LogTemp, Warning, TEXT("AMYSocket Destroyed!!!"));
		}

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(_clientSocket);

		_clientSocket = nullptr;
	}
}

void AMyFSocketActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsConnect)
	{
		int byteRead = 0;
		if (_clientSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan(1)))
		{
			if (_clientSocket->Recv(reinterpret_cast<uint8*>(__readBuffer), 4096, byteRead))
			{
				if (byteRead > 0)
				{
					int useLen = 0;
					while (useLen < byteRead)
					{
						int dataSize = byteRead - useLen;

						if (dataSize < sizeof(FPacketHeader))
							break;

						FPacketHeader* _header = reinterpret_cast<FPacketHeader*>(&__readBuffer[useLen]);
						if (dataSize < _header->size)
						{
							// 일부분만 데이터 들어오는경우 처리해야됨
							break;
						}

						// 패킷 메시지 읽기
						ReadMessageHandler(reinterpret_cast<uint8*>(&__readBuffer[useLen]), useLen);
						useLen += _header->size;
					}
				}
			}
		}
	}
}

int32 AMyFSocketActor::SendMessage(uint8* ptr, int32 count)
{
	int32 ByteSend = 0;

	if (_clientSocket)
	{
		while (ByteSend < count)
		{
			int32 _byteSent = 0;
			bool bResult = _clientSocket->Send(ptr, count, _byteSent);
			if (bResult)
				ByteSend += _byteSent;
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Socket Actor SendMessage not Send !!!!"));
				break;
			}
		}
	}
	return ByteSend;
}

void AMyFSocketActor::SetClientSocket(FSocket* ClientSocket)
{
	_clientSocket = ClientSocket;
	if (_clientSocket)
		IsConnect.Store(true);
}

void AMyFSocketActor::ReadMessageHandler(uint8* buffer, int32 size)
{
	FPacketHeader* _header = reinterpret_cast<FPacketHeader*>(buffer);
	uint16 _type = _header->type;
	uint16 _size = _header->size;
	int32 _ptr = sizeof(FPacketHeader);

	if (_header->type == 9)
	{
		UE_LOG(LogTemp, Warning, TEXT("code size : %d !!!"), _size);
	}
	// 메시지 보내는 쓰기용 버퍼도 필요!!
	switch (_type)
	{
	case 1: // 초기 데이터 로드
		{
			// 일단 정리 필요
			UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
			Aunreal_5_1_three_cGameMode* GameMode = Cast<Aunreal_5_1_three_cGameMode>(GetWorld()->GetAuthGameMode());

			uint16 playerSize = ReadBufferPtr<uint16>(buffer, _ptr);
			for (int i = 0; i < playerSize; i++)
			{
				uint16 Type = ReadBufferPtr<uint16>(buffer, _ptr);
				uint8 Hp = ReadBufferPtr<uint8>(buffer, _ptr);
				int32 Code = ReadBufferPtr<int32>(buffer, _ptr);
				int32 X = ReadBufferPtr<int32>(buffer, _ptr);
				int32 Y = ReadBufferPtr<int32>(buffer, _ptr);
				int32 Z = ReadBufferPtr<int32>(buffer, _ptr);
				float Yaw = ReadBufferPtr<float>(buffer, _ptr);
				uint16 NameLen = ReadBufferPtr<uint16>(buffer, _ptr);
				FString Name = ReadBufferStr16(buffer, _ptr, NameLen);

				TSharedPtr<PlayerInfo> PlayerInfo = MakeShared<struct PlayerInfo>(Type, Hp, Code);
				PlayerInfo->Position.X = X;
				PlayerInfo->Position.Y = Y;
				PlayerInfo->Position.Z = Z;
				PlayerInfo->Position.Yaw = Yaw;
				PlayerInfo->Name = Name;

				if (!GameMode->isSpwan.Contains(Code))
				{
					// UE_LOG(LogTemp, Log, TEXT("LOAD : %d  Type : %d !!! %s"), Code, Type, *Name);
					instance->AddPlayerInfo(PlayerInfo);
					GameMode->SpwanAnotherPlayer(PlayerInfo);
				}
			}

			uint16 monsterSize = ReadBufferPtr<uint16>(buffer, _ptr);
			for (int i = 0; i < monsterSize; i++)
			{
				uint16 Tpye = ReadBufferPtr<uint16>(buffer, _ptr);
				uint8 Hp = ReadBufferPtr<uint8>(buffer, _ptr);
				int32 Code = ReadBufferPtr<int32>(buffer, _ptr);
				int32 Target = ReadBufferPtr<int32>(buffer, _ptr);
				int32 X = ReadBufferPtr<int32>(buffer, _ptr);
				int32 Y = ReadBufferPtr<int32>(buffer, _ptr);
				int32 Z = ReadBufferPtr<int32>(buffer, _ptr);
				float Yaw = ReadBufferPtr<float>(buffer, _ptr);
				uint16 NameLen = ReadBufferPtr<uint16>(buffer, _ptr);
				FString Name = ReadBufferStr16(buffer, _ptr, NameLen);

				TSharedPtr<MonsterInfo> MonsterInfo = MakeShared<struct MonsterInfo>(Tpye, Hp, Code);
				MonsterInfo->Position.X = X;
				MonsterInfo->Position.Y = Y;
				MonsterInfo->Position.Z = Z;
				MonsterInfo->Position.Yaw = Yaw;
				MonsterInfo->Name = Name;
				MonsterInfo->target = Target;

				// UE_LOG(LogTemp, Log, TEXT("LOAD : %d HP : %d  Type : %d !!! %s"), Code, Hp, Tpye, *Name);
				// UE_LOG(LogTemp, Log, TEXT("X : %d Y : %d  Z : %d Yaw : %f"), X, Y, Z, Yaw);
				if (!GameMode->isSpwan.Contains(Code))
				{
					instance->AddMonsterInfo(MonsterInfo);
					GameMode->SpwanAnotherPlayer(MonsterInfo);
				}
			}

			// UE_LOG(LogTemp, Warning, TEXT("case 1 !!! sizef : %d"), playerSize);
			_isLoad = true;
		}
		break;
	case 2: // 플레이어 추가
		{
			uint16 Type = ReadBufferPtr<uint16>(buffer, _ptr);
			uint8 Hp = ReadBufferPtr<uint8>(buffer, _ptr);
			int32 Code = ReadBufferPtr<int32>(buffer, _ptr);
			uint16 NameLen = ReadBufferPtr<uint16>(buffer, _ptr);
			FString Name = ReadBufferStr16(buffer, _ptr, NameLen);
			TSharedPtr<PlayerInfo> PlayerInfo = MakeShared<struct PlayerInfo>(Type, Hp, Code);
			PlayerInfo->Name = Name;

			UE_LOG(LogTemp, Log, TEXT("Name : %d  Type : %d !!! %s"), Code, Type, *Name);
			Cast<UMyGameInstance>(GetGameInstance())->AddPlayerInfo(PlayerInfo);
		}
		break;
	case 3: // 이동되는 플레이어들
		{
			if (!_isLoad)
				break;

			uint16 _moveSize = ReadBufferPtr<uint16>(buffer, _ptr);
			UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
			Aunreal_5_1_three_cGameMode* GameMode = Cast<Aunreal_5_1_three_cGameMode>(GetWorld()->GetAuthGameMode());
			for (int i = 0; i < _moveSize; i++)
			{
				int32 code = ReadBufferPtr<int32>(buffer, _ptr);

				if (code >= 0 && instance->GetPlayerInfo(code))
				{
					TSharedPtr<PlayerInfo> info = instance->GetPlayerInfo(code);
					if (info == nullptr)
						continue;
					info->Code = code;
					info->Position.X = ReadBufferPtr<int32>(buffer, _ptr);
					info->Position.Y = ReadBufferPtr<int32>(buffer, _ptr);
					info->Position.Z = ReadBufferPtr<int32>(buffer, _ptr);
					info->Position.Yaw = ReadBufferPtr<float>(buffer, _ptr);

					if (!GameMode->isSpwan.Contains(code))
						GameMode->SpwanAnotherPlayer(info);
				}
				else
				{
					TSharedPtr<MonsterInfo> info = instance->GetMonsterInfo(code);
					if (info == nullptr)
						continue;
					info->Code = code;
					info->Position.X = ReadBufferPtr<int32>(buffer, _ptr);
					info->Position.Y = ReadBufferPtr<int32>(buffer, _ptr);
					info->Position.Z = ReadBufferPtr<int32>(buffer, _ptr);
					info->Position.Yaw = ReadBufferPtr<float>(buffer, _ptr);
					info->target = ReadBufferPtr<int32>(buffer, _ptr);

					if (!GameMode->isSpwan.Contains(code))
					{
						info->target = -1;
						info->SkillCode = -1;
						info->Demage = -1;
						info->Hp = 100;
						GameMode->SpwanAnotherPlayer(info);
					}
				}
			}

			// 현재 등록되지  스폰 필요 !!
		}
		break;
	case 4: // 채팅메시지
		{
			if (!_isLoad)
				break;
			int32 code = ReadBufferPtr<int32>(buffer, _ptr);
			UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
			Aunreal_5_1_three_cGameMode* GameMode = Cast<Aunreal_5_1_three_cGameMode>(GetWorld()->GetAuthGameMode());

			if (instance->GetPlayerInfo(code) || instance->_code == code)
			{
				FString _name;
				if (instance->_code != code)
					_name = instance->GetPlayerInfo(code)->Name;
				else
					_name = instance->_name;
				uint8 Type = ReadBufferPtr<uint8>(buffer, _ptr);
				uint32 TextLen = ReadBufferPtr<uint32>(buffer, _ptr);
				FString Text;
				Text.Append(_name);
				Text.Append(" : ");
				Text.Append(ReadBufferStr16(buffer, _ptr, TextLen));

				UE_LOG(LogTemp, Warning, TEXT("Converted kString !!! %s %d"), *Text, TextLen);
				GameMode->AppendChatMessage(FText::FromString(Text));
			}
		}
		break;
	case 5: // 내정보
		{
			UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
			uint16 Type = ReadBufferPtr<uint16>(buffer, _ptr);
			uint8 Hp = ReadBufferPtr<uint8>(buffer, _ptr);
			int32 Code = ReadBufferPtr<int32>(buffer, _ptr);
			uint16 NameLen = ReadBufferPtr<uint16>(buffer, _ptr);
			FString Name = ReadBufferStr16(buffer, _ptr, NameLen);

			instance->_code = Code;
			UE_LOG(LogTemp, Warning, TEXT("내정보 !!! %d"), instance->_code);
		}
		break;
	case 6: // 유저 종료 메시지
		{
			UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
			Aunreal_5_1_three_cGameMode* GameMode = Cast<Aunreal_5_1_three_cGameMode>(GetWorld()->GetAuthGameMode());
			int32 Code = ReadBufferPtr<int32>(buffer, _ptr);

			if (GameMode->isSpwan.Contains(Code))
			{
				instance->DestoryPlayer(Code);
				GameMode->DestoryAnotherPlayer(Code);
				UE_LOG(LogTemp, Warning, TEXT("disable sqwan player !!! %d"), Code);
			}
		}
		break;
	case 7: // 공격, 스킬 메시지
		{
			UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
			Aunreal_5_1_three_cGameMode* GameMode = Cast<Aunreal_5_1_three_cGameMode>(GetWorld()->GetAuthGameMode());
			int32 AttackCode = ReadBufferPtr<int32>(buffer, _ptr);
			int32 SkillCode = ReadBufferPtr<int32>(buffer, _ptr);

			if (AttackCode >= 0)
			{
				if (instance->GetPlayerInfo(AttackCode))
				{
					TSharedPtr<PlayerInfo> info = instance->GetPlayerInfo(AttackCode);
					info->SkillCode = SkillCode;
					UE_LOG(LogTemp, Warning, TEXT("유저 스킬 메시지 !!! %d"), SkillCode);

					// 여기서 플레이어 스킬 출동
				}
			}
			else
			{
				if (instance->GetMonsterInfo(AttackCode))
				{
					TSharedPtr<MonsterInfo> info = instance->GetMonsterInfo(AttackCode);
					info->SkillCode = SkillCode;
					UE_LOG(LogTemp, Warning, TEXT("유저 스킬 메시지 !!! %d"), SkillCode);

					// 여기서 몬스터 스킬 출동
				}
			}
		}
		break;
	case 8: // 히트 메시지
		{
			UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
			Aunreal_5_1_three_cGameMode* GameMode = Cast<Aunreal_5_1_three_cGameMode>(GetWorld()->GetAuthGameMode());
			int32 AttackCode = ReadBufferPtr<int32>(buffer, _ptr);
			int32 MonsterCode = ReadBufferPtr<int32>(buffer, _ptr);
			int32 Demage = ReadBufferPtr<int32>(buffer, _ptr);

			if (instance->GetMonsterInfo(MonsterCode))
			{
				TSharedPtr<MonsterInfo> info = instance->GetMonsterInfo(MonsterCode);
				info->Hit = 3;
				info->target = AttackCode;
				info->Demage = Demage;
				info->Hp -= info->Demage;
				UE_LOG(LogTemp, Warning, TEXT("몬스터 히트 메시지 !!! %d %d %d"), MonsterCode, info->target, info->Demage);

				if (info->Hp <= 0)
				{
					if (GameMode->isSpwan.Contains(MonsterCode))
					{
						GameMode->DestoryAnotherPlayer(MonsterCode);
					}
				}

				// 여기서 플레이어 스킬 출동
			}
		}
		break;
	case 9: // 공격, 스킬 메시지 다중
		{
			UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
			uint16 _attackSize = ReadBufferPtr<uint16>(buffer, _ptr);

			for (int i = 0; i < _attackSize; i++)
			{
				int32 AttackCode = ReadBufferPtr<int32>(buffer, _ptr);
				int32 SkillCode = ReadBufferPtr<int32>(buffer, _ptr);

				if (AttackCode >= 0)
				{
					if (instance->GetPlayerInfo(AttackCode))
					{
						TSharedPtr<PlayerInfo> info = instance->GetPlayerInfo(AttackCode);
						info->SkillCode = SkillCode;
						UE_LOG(LogTemp, Warning, TEXT("유저 스킬 메시지 !!! %d"), SkillCode);
					}
				}
				else
				{
					if (instance->GetMonsterInfo(AttackCode))
					{
						TSharedPtr<MonsterInfo> info = instance->GetMonsterInfo(AttackCode);
						info->SkillCode = SkillCode;
						UE_LOG(LogTemp, Warning, TEXT("몬스터 스킬 메시지 !!! %d %d"), SkillCode, _attackSize);
					}
				}
			}
		}
		break;
	}
}

FString AMyFSocketActor::ReadBufferStr(uint8* buffer, int& ptr, int32 len)
{
	FString str = FString(reinterpret_cast<char*>(&buffer[ptr]));
	ptr += len;
	return str.Mid(0, len);
}

FString AMyFSocketActor::ReadBufferStr16(uint8* buffer, int& ptr, int32 len)
{
	FString ChatString;
	ChatString.AppendChars(reinterpret_cast<const TCHAR*>(&buffer[ptr]), len / sizeof(TCHAR));
	ptr += len;
	return ChatString;
}

// 데이터 송신

// FString Message = TEXT("Hello, Server!");
// FTCHARToUTF8 Converted(*Message);
// uint16 size = (sizeof(uint16) * 2) + Converted.Length();
// uint16 id = 1;
// int32 BytesSent = 0;
//
// FPacketHeader header(id, size);
// FBufferArchive Ar;
// Ar << header.type;
// Ar << header.size;
//
// Ar.Append(Message);
//
// bool bSuccess = ClientSocket->Send(Ar.GetData(), size, BytesSent);
// bool bSuccess = ClientSocket->Send((uint8*)TCHAR_TO_UTF8(*Message), Message.Len(), BytesSent);

// if (bSuccess)
// 	UE_LOG(LogTemp, Log, TEXT("Socket Send Message %d, %lld"), size, Ar.TotalSize());


// 데이터 수신
// FPacketHeader* _header = reinterpret_cast<FPacketHeader*>(__buffer);
// uint16 _id = _header->type;
// uint16 _size = _header->size;
//
// uint16 _p = sizeof(FPacketHeader);
//
// uint16 IdLen = (static_cast<uint16>(__buffer[_p]));
// _p += sizeof(uint16);
//
// char* id_p = &__buffer[_p];
// _p += sizeof(char) * IdLen;
//
// uint16 PwLen = (static_cast<uint16>(__buffer[_p]));
// _p += sizeof(uint16);
//
// char* pw_p = &__buffer[_p];
//
// FString id_str(IdLen, id_p);
// FString pw_str(PwLen, pw_p);
//
// UE_LOG(LogTemp, Warni
