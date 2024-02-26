// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "DefaultDataRow.generated.h"

USTRUCT()
struct UNREAL_5_1_THREE_C_API FADefaultDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SkillId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillName;

};
