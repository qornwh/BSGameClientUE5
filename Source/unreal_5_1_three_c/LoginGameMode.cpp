// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"

#include "MyWdigetLogin.h"
#include "Blueprint/UserWidget.h"

ALoginGameMode::ALoginGameMode() : Super()
{
	DefaultPawnClass = nullptr;


	ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass_00(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawnBPClass_00.Succeeded())
	{
		SelectPawnList.Push(PlayerPawnBPClass_00.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass_01(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_PlayerCharacter_2.BP_PlayerCharacter_2_C'"));
	if (PlayerPawnBPClass_01.Succeeded())
	{
		SelectPawnList.Push(PlayerPawnBPClass_01.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass_02(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_PlayerCharacter_3.BP_PlayerCharacter_3_C'"));
	if (PlayerPawnBPClass_02.Succeeded())
	{
		SelectPawnList.Push(PlayerPawnBPClass_02.Class);
	}

	// static ConstructorHelpers::FClassFinder<UMyWdigetLogin> UI_HUD(
	// 	TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/WBP_LOGIN.WBP_LOGIN_C'"));
	// if (UI_HUD.Succeeded())
	// {
	// 	HUD_Class = UI_HUD.Class;
	//
	// 	LoginWidget = CreateWidget(GetWorld(), HUD_Class);
	// 	if (LoginWidget)
	// 	{
	// 		LoginWidget->AddToViewport();
	// 	}
	// }
}

ALoginGameMode::~ALoginGameMode()
{
}

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();
	// 일단 마우스 커서 보이도록 임시 처리
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	FVector Location(800.0, 0, -150);
	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Yaw = 180;
	for (auto CharacterClass : SelectPawnList)
	{
		APawn* Character = GetWorld()->SpawnActor<APawn>(CharacterClass, Location, Rotator);
		HideCharacter(Character);
		CharacterList.Add(Character);
	}
	ShowCharacter(CharacterList[CurrentIdx]);
}

void ALoginGameMode::NextCharacter()
{
	HideCharacter(CharacterList[CurrentIdx]);
	CurrentIdx++;
	CurrentIdx %= CharacterList.Num();
	ShowCharacter(CharacterList[CurrentIdx]);
}

void ALoginGameMode::PreCharacter()
{
	HideCharacter(CharacterList[CurrentIdx]);
	CurrentIdx--;
	if (CurrentIdx < 0)
		CurrentIdx = CharacterList.Num() - 1;
	ShowCharacter(CharacterList[CurrentIdx]);
}

TSubclassOf<class APawn> ALoginGameMode::GetSelectPawnClass()
{
	// 메모리 할당 해제 해야되나?? 아니면 언리얼 자체에서 GC 된다면 이거 못찾는데
	// for (int32 i = 0; i < CharacterList.Num(); i++)
	// {
	// 	delete CharacterList[i];
	// }

	return SelectPawnList[CurrentIdx];
}

uint8 ALoginGameMode::GetSelectPawnType()
{
	return CurrentIdx;
}

void ALoginGameMode::ShowCharacter(APawn* Character)
{
	// 캐릭터를 보이도록 설정
	if (Character)
	{
		Character->SetActorHiddenInGame(false);
		Character->SetActorEnableCollision(false);
		Character->SetActorTickEnabled(false);
	}
}

void ALoginGameMode::HideCharacter(APawn* Character)
{
	// 캐릭터를 숨기도록 설정
	if (Character)
	{
		Character->SetActorHiddenInGame(true);
		Character->SetActorEnableCollision(false);
		Character->SetActorTickEnabled(false);
	}
}
