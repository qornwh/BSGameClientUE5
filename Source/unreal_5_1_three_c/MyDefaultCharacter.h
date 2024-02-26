// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyDefaultCharacter.generated.h"

UCLASS()
class UNREAL_5_1_THREE_C_API AMyDefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyDefaultCharacter();
	~AMyDefaultCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetPlayerInfo(TSharedPtr<struct PlayerInfo> PlayerInfo);
	void SetMonsterInfo(TSharedPtr<struct MonsterInfo> MonsterInfo);

	TSharedPtr<struct UnitInfo> GetUnitInfo();

	void RotateActor(float Yew);
	void MonsterRotate(FVector nextFVector);

	FVector TargetMove(int32 target);

	void HitDemagePlay(int32 demage);

private:
	float GetRotate(FVector& next);

	TSharedPtr<struct UnitInfo> _unitInfo;

	UPROPERTY(EditAnywhere)
	float _tickTime = 0.3f;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* NameUi;

	UPROPERTY()
	class UAMyDefaultCharatorAnimInstance* AnimInstance;

	bool interpolation = true;
	
	bool testFlag = true;
};
