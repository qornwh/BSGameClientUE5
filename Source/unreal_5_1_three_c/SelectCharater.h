// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SelectCharater.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_5_1_THREE_C_API ASelectCharater : public APlayerController
{
	GENERATED_BODY()

public:
	ASelectCharater();
	~ASelectCharater() {};
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	TSubclassOf<UUserWidget> HUD_Class;

	UPROPERTY()
	UUserWidget* LoginWidget;
};
