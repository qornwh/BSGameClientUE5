// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyWdigetLogin.generated.h"

/**
 * 
 */

// DECLARE_DELEGATE_TwoParams(D_Login_Send, const FString&, const FString&)

UCLASS()
class UNREAL_5_1_THREE_C_API UMyWdigetLogin : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* Login_ID;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Login_BTN;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Type_Left_BTN;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* Type_Right_BTN;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnLoginEvent();

	UFUNCTION()
	void OnLeftClickEvent();

	UFUNCTION()
	void OnRightClickEvent();

	// D_Login_Send Login_Send;

	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

private:
	int32 idx = 0;
};
