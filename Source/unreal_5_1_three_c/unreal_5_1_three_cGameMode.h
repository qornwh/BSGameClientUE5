// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MyFSocketActor.h"
#include "GameFramework/GameModeBase.h"
#include "unreal_5_1_three_cGameMode.generated.h"

UCLASS(minimalapi)
class Aunreal_5_1_three_cGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	Aunreal_5_1_three_cGameMode();
	virtual void StartPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void RestartPlayer(AController* NewPlayer) override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACharacter> monsterBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACharacter> AnotherPlayerBP;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> DefaultMagicBallBP;
	
	UPROPERTY()
	TSubclassOf<class UUserWidget> HUD_Class;

	UPROPERTY()
	UUserWidget* MainWidget;
	void AppendChatMessage(FText text);

	UPROPERTY()
	TSubclassOf<class UUserWidget> UI_Demage;

	TSubclassOf<class AActor> socketActor;

	AMyFSocketActor* GetFScoketActor();

	void SpwanAnotherPlayer(TSharedPtr<struct UnitInfo> info);

	void DestoryAnotherPlayer(int32 Code);

	void InitAnotherPawnClass();

private:
	AMyFSocketActor* MyFSocketActor;

	TMap<uint16, TSubclassOf<class APawn>> SelectPawnDic;

public:
	TMap<int32, AActor*> isSpwan;
};
