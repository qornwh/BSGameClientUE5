// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/LevelStreamingDynamic.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_5_1_THREE_C_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();
	~UMyGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	FSocket* GetClientSocket();

	void ConnectClientSocket();
	
	void SetUserInfo(FString& id);

	void AddPlayerInfo(TSharedPtr<struct PlayerInfo> Info);
	TSharedPtr<struct PlayerInfo> GetPlayerInfo(int32 Code);

	void AddMonsterInfo(TSharedPtr<struct MonsterInfo> Info);
	TSharedPtr<struct MonsterInfo> GetMonsterInfo(int32 Code);

	void SendMessage();

	void DestoryPlayer(int32 Code);

	void SetDefaultPawnClass(TSubclassOf<class APawn> DefaultCharaterClass);
	TSubclassOf<class APawn> GetDefaultPawnClass();
	
	void SetDefaultPawnType(uint16 Type);
	uint16 GetDefaultPawnType();

private:
	// 스마트 포인터로 바꾸자
	FSocket* _clientSocket;
	FTimerHandle _timerHandle;
	FTimerHandle _timerMoveHandle;
	bool isConnect;
	int16 count = 5;

	ULevelStreamingDynamic* mainLevel;

	void CountdownHandler();
	void CountdownHasFinished();

public:
	// 유저정보 정리 해야됨
	FString _name;

	uint32 _code;
	uint16 _type = 0;

private:
	TMap<int32,TSharedPtr<struct PlayerInfo>> PlayerInfos;
	TMap<int32,TSharedPtr<struct MonsterInfo>> MonsterInfos;

	TSubclassOf<class APawn> _DefaultCharaterClass = nullptr; 
};
