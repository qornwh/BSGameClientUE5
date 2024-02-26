// Fill out your copyright notice in the Description page of Project Settings.


#include "AMyDefaultCharatorAnimInstance.h"

#include "DefaultDataRow.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UAMyDefaultCharatorAnimInstance::UAMyDefaultCharatorAnimInstance()
{
	CurrentPwanSpeed = 0.f;
	IsFalling = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(
		TEXT("/Script/Engine.AnimMontage'/Game/ThirdPerson/Animation/AM_Attack.AM_Attack'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Monster00(
		TEXT("/Script/Engine.AnimMontage'/Game/ThirdPerson/Animation/AM_Cactus.AM_Cactus'"));
	if (AM_Monster00.Succeeded())
	{
		AM_Map.Add(10, AM_Monster00.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Monster01(
		TEXT("/Script/Engine.AnimMontage'/Game/ThirdPerson/Animation/AM_Salamander.AM_Salamander'"));
	if (AM_Monster01.Succeeded())
	{
		AM_Map.Add(11, AM_Monster01.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Monster02(
		TEXT("/Script/Engine.AnimMontage'/Game/ThirdPerson/Animation/AM_BishopKnight.AM_BishopKnight'"));
	if (AM_Monster02.Succeeded())
	{
		AM_Map.Add(12, AM_Monster02.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Monster03(
		TEXT("/Script/Engine.AnimMontage'/Game/ThirdPerson/Animation/AM_StringRay.AM_StringRay'"));
	if (AM_Monster03.Succeeded())
	{
		AM_Map.Add(13, AM_Monster03.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Monster04(
		TEXT("/Script/Engine.AnimMontage'/Game/ThirdPerson/Animation/AM_Cyclops.AM_Cyclops'"));
	if (AM_Monster04.Succeeded())
	{
		AM_Map.Add(14, AM_Monster04.Object);
	}
	LoadCode();
}

void UAMyDefaultCharatorAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		CurrentPwanSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsFalling = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UAMyDefaultCharatorAnimInstance::PlayAttackMontage(float rate)
{
	Montage_Play(AttackMontage, rate);
}

void UAMyDefaultCharatorAnimInstance::JumpToAttack(int32 SkillID)
{
	if (SkillCodeMap.Contains(SkillID))
	{
		Montage_JumpToSection(*SkillCodeMap[SkillID], AttackMontage);
	}
}

void UAMyDefaultCharatorAnimInstance::SetAttacking(bool Attacking)
{
	IsAttacking = Attacking;
}

bool UAMyDefaultCharatorAnimInstance::GetAttacking()
{
	return IsAttacking;
}

void UAMyDefaultCharatorAnimInstance::SetAttackMontage(int32 Code)
{
	if (AM_Map.Contains(Code) && AM_Map[Code])
	{
		AttackMontage = AM_Map[Code];
		UE_LOG(LogTemp, Warning, TEXT("SetAttackMontage %d !!!"), Code);
	}
}

void UAMyDefaultCharatorAnimInstance::LoadCode()
{
	if (SkillCodeMap.Num() > 0)
		return;

	static ConstructorHelpers::FObjectFinder<UDataTable> CodeData(
		TEXT("/Script/Engine.DataTable'/Game/ThirdPerson/Data/Skill.Skill'"));
	TArray<FADefaultDataRow> DataRows;
	if (CodeData.Succeeded())
	{
		UDataTable* dataTable = CodeData.Object;

		for (auto name : dataTable->GetRowNames())
		{
			FADefaultDataRow* DataRow = dataTable->FindRow<FADefaultDataRow>(name, TEXT(""));
			if (DataRow == nullptr)
				continue;
			// UE_LOG(LogTemp, Log, TEXT("GetRowNames !!! %s"), *name.ToString());
			DataRows.Add(*DataRow);
		}
	}

	for (auto& data : DataRows)
	{
		SkillCodeMap.Add(data.SkillId, data.SkillName);
		// UE_LOG(LogTemp, Log, TEXT("data !!! %d %s"), data.SkillId, *SkillCodeMap[data.SkillId]);
	}
}

void UAMyDefaultCharatorAnimInstance::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	UE_LOG(LogTemp, Log, TEXT("OnAttackMontageEnded !!!"));
	// 데미지 계산
	// OnAttacked.Broadcast();
}

void UAMyDefaultCharatorAnimInstance::SetHetting(bool Moving)
{
	IsHitting = Moving;
}

bool UAMyDefaultCharatorAnimInstance::GetMoving()
{
	return !(IsHitting || IsAttacking);
}

void UAMyDefaultCharatorAnimInstance::AnimNotify_AttackHit()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit"));
	OnAttackHit.Broadcast();
}

void UAMyDefaultCharatorAnimInstance::OnHitMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsHitting = false;
	UE_LOG(LogTemp, Log, TEXT("OnHitMontageEnded !!!"));
}
