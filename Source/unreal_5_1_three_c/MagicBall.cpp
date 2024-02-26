// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicBall.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMagicBall::AMagicBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	// Collision->OnComponentBeginOverlap

	RootComponent = Collision;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.f;
	Movement->SetUpdatedComponent(RootComponent);
}

// Called when the game starts or when spawned
void AMagicBall::BeginPlay()
{
	Super::BeginPlay();

	_startPos = GetActorLocation();
}

// Called every frame
void AMagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLoc = GetActorLocation();
	float Distance = FMath::Sqrt(FMath::Pow(CurrentLoc.X - _startPos.X, 2) + FMath::Pow(CurrentLoc.Y - _startPos.Y, 2));

	if (Distance >= AttackRange)
		Destroy();
}

void AMagicBall::SetVelocity(const FVector& Driection) const
{
	Movement->Velocity = Driection * Speed;
}
