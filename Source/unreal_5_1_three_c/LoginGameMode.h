// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_5_1_THREE_C_API ALoginGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALoginGameMode();
	~ALoginGameMode();

	virtual void BeginPlay() override;
	void NextCharacter();
	void PreCharacter();
	TSubclassOf<class APawn> GetSelectPawnClass();

	uint8 GetSelectPawnType();

private:
	void ShowCharacter(APawn* Character);
	void HideCharacter(APawn* Character);

private:
	TSubclassOf<class APawn> SelectCharacter;

	TArray<TSubclassOf<class APawn>> SelectPawnList;
	TArray<APawn*> CharacterList;

	int8 CurrentIdx;
	//
// public:
// 	UPROPERTY()
// 	TSubclassOf<UUserWidget> HUD_Class;
//
// 	UPROPERTY()
// 	UUserWidget* LoginWidget;
};
