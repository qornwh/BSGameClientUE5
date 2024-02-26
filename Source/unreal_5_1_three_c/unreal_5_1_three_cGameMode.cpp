// Copyright Epic Games, Inc. All Rights Reserved.

#include "unreal_5_1_three_cGameMode.h"

#include "MainInterface.h"
#include "MyDefaultCharacter.h"
#include "MyDemageWidget.h"
#include "MyFSocketActor.h"
#include "MyGameInstance.h"
#include "unreal_5_1_three_cCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Struct/PlayerInfo.h"

Aunreal_5_1_three_cGameMode::Aunreal_5_1_three_cGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter_C'"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FObjectFinder<UClass> SocketActor(
		TEXT("/Script/CoreUObject.Class'/Script/unreal_5_1_three_c.MyFSocketActor'"));
	if (SocketActor.Object)
	{
		socketActor = (UClass*)SocketActor.Object;
	}
	static ConstructorHelpers::FClassFinder<UMainInterface> UI_HUD(TEXT(
		"/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/WBP_MAININTERFACE.WBP_MAININTERFACE_C'"));
	if (UI_HUD.Succeeded())
	{
		HUD_Class = UI_HUD.Class;

		MainWidget = CreateWidget(GetWorld(), HUD_Class);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}
	static ConstructorHelpers::FClassFinder<UMyDemageWidget> UI_DEMAGE(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/WBP_Demage.WBP_Demage_C'"));
	if (UI_DEMAGE.Succeeded())
	{
		UI_Demage = UI_DEMAGE.Class;
	}
	static ConstructorHelpers::FClassFinder<AActor> MgaicBallClass(
		TEXT("/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_MagicBall.BP_MagicBall_C'"));
	if (MgaicBallClass.Succeeded())
	{
		DefaultMagicBallBP = MgaicBallClass.Class;
	}
	InitAnotherPawnClass();
}

void Aunreal_5_1_three_cGameMode::StartPlay()
{
	Super::StartPlay();
	UE_LOG(LogTemp, Log, TEXT("StartPlay !!!"));

	if (monsterBP != nullptr)
	{
		// FVector Location(2200, 700, 300);
		// ACharacter* AICharacter = GetWorld()->SpawnActor<ACharacter>(monsterBP, Location, FRotator::ZeroRotator);
		//
		// if (AICharacter)
		// {
		// 	UE_LOG(LogTemp, Log, TEXT("MyAICharater befor Spawn!!!"));
		// }
	}

	if (socketActor != nullptr)
	{
		FVector Location(2200, 700, 3000);
		AActor* actor = GetWorld()->SpawnActor<AActor>(socketActor, Location, FRotator::ZeroRotator);
		MyFSocketActor = Cast<AMyFSocketActor>(actor);
		UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());

		if (MyFSocketActor && instance)
		{
			MyFSocketActor->SetClientSocket(instance->GetClientSocket());
			UE_LOG(LogTemp, Log, TEXT("MySocketActor befor Spawn!!!"));
		}
	}
}

void Aunreal_5_1_three_cGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());

	if (instance && instance->GetDefaultPawnClass() != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("GetDefaultPawnClass !!!"));
		DefaultPawnClass = instance->GetDefaultPawnClass();
	}
}

void Aunreal_5_1_three_cGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	UE_LOG(LogTemp, Log, TEXT("RestartPlayer !!!"));
}

AMyFSocketActor* Aunreal_5_1_three_cGameMode::GetFScoketActor()
{
	return MyFSocketActor;
}

void Aunreal_5_1_three_cGameMode::SpwanAnotherPlayer(TSharedPtr<UnitInfo> info)
{
	FVector Location(info->Position.X, info->Position.Y, 300.f);
	FRotator Rotator = FRotator::ZeroRotator;
	Rotator.Yaw = info->Position.Yaw;
	if (info->Type < 20 && SelectPawnDic.Contains(info->Type))
	{
		AMyDefaultCharacter* AnotherPlayer = GetWorld()->SpawnActor<AMyDefaultCharacter>(
			SelectPawnDic[info->Type], Location, Rotator);

		if (AnotherPlayer)
		{
			isSpwan.Add(info->Code, Cast<AActor>(AnotherPlayer));
			if (info->Code >= 0)
			{
				UE_LOG(LogTemp, Log, TEXT("Spwan Actor %d !!!"), info->Code);
				AnotherPlayer->SetPlayerInfo(StaticCastSharedPtr<PlayerInfo>(info));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Spwan Actor %d !!!"), info->Code);
				AnotherPlayer->SetMonsterInfo(StaticCastSharedPtr<MonsterInfo>(info));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("not Spwan Actor !!!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("해당하는 타입의 폰 클래스가 없습니다 %d !!!"), info->Type);
	}
}

void Aunreal_5_1_three_cGameMode::DestoryAnotherPlayer(int32 Code)
{
	isSpwan[Code]->Destroy();
	isSpwan.Remove(Code);
	UE_LOG(LogTemp, Warning, TEXT("액터 Distroy %d !!!"), Code);
}

void Aunreal_5_1_three_cGameMode::InitAnotherPawnClass()
{
	ConstructorHelpers::FClassFinder<APawn> AnotherPawnBPClass_00(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_AnotherCharater.BP_AnotherCharater_C'"));
	if (AnotherPawnBPClass_00.Succeeded())
	{
		SelectPawnDic.Add(0, AnotherPawnBPClass_00.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> AnotherPawnBPClass_01(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_AnotherCharater_2.BP_AnotherCharater_2_C'"));
	if (AnotherPawnBPClass_01.Succeeded())
	{
		SelectPawnDic.Add(1, AnotherPawnBPClass_01.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> AnotherPawnBPClass_02(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/BP_AnotherCharater_3.BP_AnotherCharater_3_C'"));
	if (AnotherPawnBPClass_02.Succeeded())
	{
		SelectPawnDic.Add(2, AnotherPawnBPClass_02.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> MonsterPawnBPClass_00(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/Monster/BP_Cactus.BP_Cactus_C'"));
	if (MonsterPawnBPClass_00.Succeeded())
	{
		SelectPawnDic.Add(10, MonsterPawnBPClass_00.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> MonsterPawnBPClass_01(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/Monster/BP_Salamander.BP_Salamander_C'"));
	if (MonsterPawnBPClass_01.Succeeded())
	{
		SelectPawnDic.Add(11, MonsterPawnBPClass_01.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> MonsterPawnBPClass_02(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/Monster/BP_BishopKnight.BP_BishopKnight_C'"));
	if (MonsterPawnBPClass_02.Succeeded())
	{
		SelectPawnDic.Add(12, MonsterPawnBPClass_02.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> MonsterPawnBPClass_03(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/Monster/BP_StingRay.BP_StingRay_C'"));
	if (MonsterPawnBPClass_03.Succeeded())
	{
		SelectPawnDic.Add(13, MonsterPawnBPClass_03.Class);
	}
	ConstructorHelpers::FClassFinder<APawn> MonsterPawnBPClass_04(
		TEXT(
			"/Script/Engine.Blueprint'/Game/ThirdPerson/Blueprints/Monster/BP_Cyclops.BP_Cyclops_C'"));
	if (MonsterPawnBPClass_04.Succeeded())
	{
		SelectPawnDic.Add(14, MonsterPawnBPClass_04.Class);
	}
}

void Aunreal_5_1_three_cGameMode::AppendChatMessage(FText text)
{
	Cast<UMainInterface>(MainWidget)->AppendText(text);
}
