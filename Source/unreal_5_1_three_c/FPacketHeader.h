// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class UNREAL_5_1_THREE_C_API FPacketHeader
{
public:
	FPacketHeader(uint16 type, uint16 size);
	~FPacketHeader();

public:
	uint16 type = 0;
	uint16 size = 0;
};
