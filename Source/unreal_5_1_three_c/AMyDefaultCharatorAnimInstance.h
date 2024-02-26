// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AMyDefaultCharatorAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);

/**
 * 
 */
UCLASS()
class UNREAL_5_1_THREE_C_API UAMyDefaultCharatorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAMyDefaultCharatorAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage(float rate = 1.f);
	void JumpToAttack(int32 SkillID);

	void SetAttacking(bool Attacking);
	bool GetAttacking();

	void SetAttackMontage(int32 Code);
	
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void SetHetting(bool Moving);
	bool GetMoving();
	
	UFUNCTION()
	void AnimNotify_AttackHit();
	UFUNCTION()
	void OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	float CurrentPwanSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	bool IsFalling;
	
	UPROPERTY(VisibleAnywhere, Category=Pawn, Meta=(AllowPrivateAccess=true))
	bool IsAttacking = false;
	
	UPROPERTY(VisibleAnywhere, Category=Pawn, Meta=(AllowPrivateAccess=true))
	bool IsHitting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=true))
	UAnimMontage* AttackMontage;

	void LoadCode();

public:
	FOnAttackHit OnAttackHit;

private:
	TMap<int32, UAnimMontage*> AM_Map;
	TMap<int32, FString> SkillCodeMap;
};
