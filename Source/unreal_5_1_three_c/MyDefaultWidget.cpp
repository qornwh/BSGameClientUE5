// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDefaultWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMyDefaultWidget::SetText(FText str)
{
	NAMEUI->SetText(str);
}

void UMyDefaultWidget::SetHp(float ratio)
{
	// 딜계산 비율
	HP->SetPercent(ratio);
}
