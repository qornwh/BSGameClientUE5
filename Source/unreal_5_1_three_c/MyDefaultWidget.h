// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyDefaultWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_5_1_THREE_C_API UMyDefaultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* NAMEUI;
	
	UPROPERTY(meta=(BindWidget))
	class UProgressBar* HP;
	
	void SetText(FText str);
	void SetHp(float ratio);
};
