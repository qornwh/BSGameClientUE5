// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharater.h"

#include "MyWdigetLogin.h"

ASelectCharater::ASelectCharater()
{

	static ConstructorHelpers::FClassFinder<UMyWdigetLogin> UI_HUD(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/WBP_LOGIN.WBP_LOGIN_C'"));
	if (UI_HUD.Succeeded())
	{
		HUD_Class = UI_HUD.Class;

		LoginWidget = CreateWidget(GetWorld(), HUD_Class);
		if (LoginWidget)
		{
			LoginWidget->AddToViewport();
		}
	}
}

void ASelectCharater::BeginPlay()
{
	Super::BeginPlay();
}
