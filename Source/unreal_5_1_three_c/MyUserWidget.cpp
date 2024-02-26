// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"

#include "Components/TextBlock.h"

void UMyUserWidget::SetText(FText str)
{
	NAMEUI->SetText(str);
}
