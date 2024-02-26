// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicBall.generated.h"

UCLASS()
class UNREAL_5_1_THREE_C_API AMagicBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// 콜리전 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;
	
	// 무브먼트 컴포넌트
	UPROPERTY(EditAnywhere, Category = "Properties", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Magic)
	float Speed = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Magic)
	float AttackRange = 500.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetVelocity(const FVector& Driection) const;

private:
	FVector _startPos;
	float _yaw;
};
