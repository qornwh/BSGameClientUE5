// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWdigetLogin.h"

#include "LoginGameMode.h"
#include "MyGameInstance.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UMyWdigetLogin::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Log, TEXT("NativeOnInitialized !!!!"));

	Login_BTN->OnClicked.AddDynamic(this, &UMyWdigetLogin::OnLoginEvent);
	Type_Left_BTN->OnClicked.AddDynamic(this, &UMyWdigetLogin::OnLeftClickEvent);
	Type_Right_BTN->OnClicked.AddDynamic(this, &UMyWdigetLogin::OnRightClickEvent);

	// UMyGameInstance* instance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	// Login_Send.BindUObject(instance, &UMyGameInstance::SetUserInfo);
}

void UMyWdigetLogin::OnLoginEvent()
{
	UE_LOG(LogTemp, Log, TEXT("Login EVENT !!!!"));

	UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());
	if (instance != nullptr)
	{
		instance->_name = Login_ID->GetText().ToString();
		instance->ConnectClientSocket();
		ALoginGameMode* Mode = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());
		if (Mode != nullptr)
		{
			instance->SetDefaultPawnClass(Mode->GetSelectPawnClass());
			instance->SetDefaultPawnType(Mode->GetSelectPawnType());
		}
	}
}

void UMyWdigetLogin::OnLeftClickEvent()
{
	ALoginGameMode* Mode = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());
	if (Mode != nullptr)
	{
		Mode->PreCharacter();
	}
}

void UMyWdigetLogin::OnRightClickEvent()
{
	ALoginGameMode* Mode = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());
	if (Mode != nullptr)
	{
		Mode->NextCharacter();
	}
}
