// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ScrollBox.h"
#include "MainInterface.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_5_1_THREE_C_API UMainInterface : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Event")
	void OnEventKeyPressed();
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Send_BTN;

	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* Editor_TB;
	
	UPROPERTY(meta=(BindWidget))
	class UScrollBox* MainChat_SB;
	void AppendText(FText text);

	UFUNCTION(BlueprintCallable, Category="Event")
	void SendTextMsg();

protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
