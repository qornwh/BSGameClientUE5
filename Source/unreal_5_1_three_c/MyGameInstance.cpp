// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

#include "FPacketHandler.h"
#include "FPacketHeader.h"
#include "SocketSubsystem.h"
#include "unreal_5_1_three_cCharacter.h"
#include "unreal_5_1_three_cGameMode.h"
#include "Common/TcpSocketBuilder.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Kismet/GameplayStatics.h"
#include "Protocol\P_PROTOCOL_PAKCET.h"
#include "Struct/PlayerInfo.h"

UMyGameInstance::UMyGameInstance()
{
}

UMyGameInstance::~UMyGameInstance()
{
}

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Log, TEXT("UMyGameInstance Init !!!"));
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
}

FSocket* UMyGameInstance::GetClientSocket()
{
	return _clientSocket;
}

void UMyGameInstance::ConnectClientSocket()
{
	isConnect = false;
	const int32 port = 12127;
	const FString Address = TEXT("172.30.1.53");
	FIPv4Address Ip;
	FIPv4Address::Parse(Address, Ip); // 서버의 IP 주소 입력

	TSharedRef<FInternetAddr> ServerAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	ServerAddress->SetIp(Ip.Value);
	ServerAddress->SetPort(port); // 서버 포트 번호 설정

	_clientSocket = FTcpSocketBuilder(TEXT("MyFSocketActor")).AsNonBlocking().AsReusable();
	// _clientSocket->SetNonBlocking(false);
	_clientSocket->Connect(*ServerAddress);

	UE_LOG(LogTemp, Warning, TEXT("socket connection loading !!!"));

	GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &UMyGameInstance::CountdownHandler, 2.f, true);
}

void UMyGameInstance::SetUserInfo(FString& id)
{
	_name = id;
}

void UMyGameInstance::AddPlayerInfo(TSharedPtr<PlayerInfo> Info)
{
	PlayerInfos.Add(Info->Code, Info);
}

TSharedPtr<PlayerInfo> UMyGameInstance::GetPlayerInfo(int32 Code)
{
	if (PlayerInfos.Contains(Code))
	{
		return PlayerInfos[Code];
	}
	return nullptr;
}

void UMyGameInstance::AddMonsterInfo(TSharedPtr<MonsterInfo> Info)
{
	MonsterInfos.Add(Info->Code, Info);
}

TSharedPtr<MonsterInfo> UMyGameInstance::GetMonsterInfo(int32 Code)
{
	if (MonsterInfos.Contains(Code))
	{
		return MonsterInfos[Code];
	}
	return nullptr;
}

void UMyGameInstance::SendMessage()
{
	Aunreal_5_1_three_cCharacter* player = Cast<Aunreal_5_1_three_cCharacter>(
		GetWorld()->GetFirstPlayerController()->GetCharacter());

	if (player)
		player->MovePlayer.Execute();
}

void UMyGameInstance::DestoryPlayer(int32 Code)
{
	PlayerInfos.Remove(Code);
}

void UMyGameInstance::SetDefaultPawnClass(TSubclassOf<class APawn> DefaultCharaterClass)
{
	_DefaultCharaterClass = DefaultCharaterClass;
}

void UMyGameInstance::SetDefaultPawnType(uint16 Type)
{
	_type = Type;
}

uint16 UMyGameInstance::GetDefaultPawnType()
{
	return _type;
}

TSubclassOf<class APawn> UMyGameInstance::GetDefaultPawnClass()
{
	return _DefaultCharaterClass;
}

void UMyGameInstance::CountdownHandler()
{
	count--;

	if (_clientSocket)
	{
		UE_LOG(LogTemp, Warning, TEXT("socket CountdownHandler loading !!!"));
		if (_clientSocket->GetConnectionState() == SCS_Connected)
		{
			isConnect = true;
			GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
			CountdownHasFinished();
		}
		if (count <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
			CountdownHasFinished();
		}
	}
}

void UMyGameInstance::CountdownHasFinished()
{
	if (isConnect)
	{
		UE_LOG(LogTemp, Warning, TEXT("socket connection success !!!"));

		P_LOGIN_PAKCET pkt;
		FBufferArchive Ar;
		pkt.Id = _name;
		pkt.IdLen = _name.Len() * sizeof(TCHAR);
		pkt.Type = GetDefaultPawnType();

		int32 byteSent = 0;
		// 끝에 2바이트는 fstring 마지막 /0 이라서 2개이므로
		int32 size = pkt.Size() + sizeof(FPacketHeader);
		uint8* stream = FPacketHandler::MakePacket(pkt, Ar);

		while(byteSent < size)
		{
			int32 _byteSent = 0;
			bool bReault = _clientSocket->Send(stream, size, _byteSent);
			if (bReault)
				byteSent += _byteSent;
			else
				UE_LOG(LogTemp, Error, TEXT("Socket SendMessage not Send !!!!"));
		}

		UGameplayStatics::OpenLevel(
			GetWorld(), FName("Game"), true);

		GetWorld()->GetTimerManager().SetTimer(_timerMoveHandle, this, &UMyGameInstance::SendMessage, 0.3f, true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("socket connection failed !!!"));
	}
}
