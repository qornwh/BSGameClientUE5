// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDefaultCharacter.h"

#include "AMyDefaultCharatorAnimInstance.h"
#include "MagicBall.h"
#include "MyDefaultWidget.h"
#include "MyDemageWidget.h"
#include "MyGameInstance.h"
#include "unreal_5_1_three_cCharacter.h"
#include "unreal_5_1_three_cGameMode.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/PlayerInfo.h"

// Sets default values
AMyDefaultCharacter::AMyDefaultCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// PrimaryActorTick.TickInterval = _tickTime;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DefaultCharactor"));

	static ConstructorHelpers::FClassFinder<UMyDefaultWidget> UW(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/WBP_NAMEHP.WBP_NAMEHP_C'"));
	if (UW.Succeeded())
	{
		NameUi = CreateDefaultSubobject<UWidgetComponent>(TEXT("NAMEUI"));
		NameUi->SetupAttachment(GetMesh());
		NameUi->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
		NameUi->SetWidgetSpace(EWidgetSpace::Screen);

		NameUi->SetWidgetClass(UW.Class);
		NameUi->SetDrawSize(FVector2D(300.f, 50.f));
	}
}

AMyDefaultCharacter::~AMyDefaultCharacter()
{
	_unitInfo = nullptr;
}

// Called when the game starts or when spawned
void AMyDefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UAMyDefaultCharatorAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(AnimInstance, &UAMyDefaultCharatorAnimInstance::OnHitMontageEnded);
		AnimInstance->OnMontageEnded.AddDynamic(AnimInstance, &UAMyDefaultCharatorAnimInstance::OnAttackMontageEnded);
		// 공격 디버깅 어디에 찍혔는지
		// AnimInstance->OnAttackHit.AddUObject(this, &UAMyDefaultCharatorAnimInstance::AttackCheck);
	}
}

// Called every frame
void AMyDefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_unitInfo)
	{
		// 스킬 액션을 플레이한다.
		if (_unitInfo->SkillCode > 0 && GetVelocity().SizeSquared() <= 0.0f)
		{
			if (_unitInfo->Code < 0)
			{
				MonsterRotate(FVector::ZeroVector);
			}
			// 그냥 실행 시키고 본다.

			if (_unitInfo->Code > 0)
			{
				// 플레이어만
				Aunreal_5_1_three_cGameMode* mode = Cast<
					Aunreal_5_1_three_cGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (mode != nullptr && mode->DefaultMagicBallBP != nullptr)
				{
					// magicBall생성
					AMagicBall* ball = GetWorld()->SpawnActor<AMagicBall>(
						mode->DefaultMagicBallBP, GetActorLocation(), FRotator::ZeroRotator);
					if (ball != nullptr)
					{
						ball->SetVelocity(GetActorForwardVector());
					}
				}
			}

			AnimInstance->SetAttacking(true);
			AnimInstance->PlayAttackMontage();
			AnimInstance->JumpToAttack(_unitInfo->SkillCode);
			_unitInfo->SkillCode = -1;
		}
		else if (_unitInfo->Hit > 0 && _unitInfo->Demage > 0)
		{
			// 그냥 실행 시키고 본다
			AnimInstance->PlayAttackMontage();
			AnimInstance->JumpToAttack(_unitInfo->Hit);
			AnimInstance->SetHetting(true);
			HitDemagePlay(_unitInfo->Demage);
			_unitInfo->Hit = -1;
			_unitInfo->Demage = -1;
		}
		else
		{
			if (AnimInstance->GetMoving())
			{
				// FinalMovement를 사용하여 Pawn을 이동시킵니다.
				FVector currentFVector = GetActorLocation();
				FVector destFVector(_unitInfo->Position.X, _unitInfo->Position.Y, currentFVector.Z);

				FVector nextFVector = (destFVector - currentFVector).GetSafeNormal();
				if (!currentFVector.Equals(destFVector, 10))
				{
					interpolation = true;
					if (_unitInfo->Code >= 0 && currentFVector.Equals(destFVector, 30))
					{
						double newYew = _unitInfo->Position.Yaw - GetActorRotation().Yaw;
						RotateActor(newYew);

						AddMovementInput(nextFVector, 0.3, true);
					}
					else
					{
						double x = abs(destFVector.X - currentFVector.X);
						double y = abs(destFVector.Y - currentFVector.Y);

						// 거리따라 속도 조절
						float _dist = sqrt(pow(x, 2) + pow(y, 2));
						if (_unitInfo->Code >= 0)
						{
							// 플레이어인 경우
							float newYew = _unitInfo->Position.Yaw - GetActorRotation().Yaw;
							RotateActor(newYew);
							AddMovementInput(nextFVector, _unitInfo->Speed * _dist / 100, true);
						}
						else
						{
							// 몬스터인 경우
							int32 target = StaticCastSharedPtr<MonsterInfo>(_unitInfo)->target;
							float newYaw;
							if (target >= 0)
							{
								FVector TargetFVector = TargetMove(target);
								newYaw = GetRotate(TargetFVector);
							}
							else
							{
								newYaw = GetRotate(nextFVector);
							}

							RotateActor(newYaw);
							AddMovementInput(nextFVector, _unitInfo->Speed * _dist / 100, true);;
						}
					}
				}
				else if (_unitInfo->Code >= 0)
				{
					if (interpolation)
					{
						float Yaw = _unitInfo->Position.Yaw - GetActorRotation().Yaw;
						RotateActor(Yaw);
						interpolation = false;
					}
				}
			}
		}
	}
}

// Called to bind functionality to input
void AMyDefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyDefaultCharacter::SetPlayerInfo(TSharedPtr<PlayerInfo> PlayerInfo)
{
	_unitInfo = PlayerInfo;

	if (NameUi)
	{
		auto NameWidget = Cast<UMyDefaultWidget>(NameUi->GetUserWidgetObject());
		FString name = _unitInfo->Name;
		NameWidget->SetText(FText::FromString(name));
		NameWidget->SetHp(1.0);
	}
}

void AMyDefaultCharacter::SetMonsterInfo(TSharedPtr<MonsterInfo> MonsterInfo)
{
	_unitInfo = MonsterInfo;

	if (NameUi)
	{
		auto NameWidget = Cast<UMyDefaultWidget>(NameUi->GetUserWidgetObject());
		FString name = _unitInfo->Name;
		NameWidget->SetText(FText::FromString(name));
		NameWidget->SetHp(1.0);
	}

	if (_unitInfo->Code < 0)
	{
		AnimInstance->SetAttackMontage(_unitInfo->Type);
	}
}

TSharedPtr<UnitInfo> AMyDefaultCharacter::GetUnitInfo()
{
	return _unitInfo;
}

void AMyDefaultCharacter::RotateActor(float Yew)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += Yew;
	SetActorRotation(NewRotation);
}

void AMyDefaultCharacter::MonsterRotate(FVector nextFVector)
{
	int32 target = StaticCastSharedPtr<MonsterInfo>(_unitInfo)->target;
	float newYaw = 0;
	if (target >= 0)
	{
		FVector TargetFVector = TargetMove(target);
		newYaw = GetRotate(TargetFVector);
	}
	else
	{
		newYaw = GetRotate(nextFVector);
	}
	RotateActor(newYaw);
}

FVector AMyDefaultCharacter::TargetMove(int32 target)
{
	FVector currentFVector = GetActorLocation();
	FVector destFVector(FVector::ZeroVector);
	destFVector.Z = currentFVector.Z;
	FVector nextFVector = currentFVector;

	UMyGameInstance* _instance = Cast<UMyGameInstance>(GetGameInstance());
	if (_instance->GetPlayerInfo(target))
	{
		BS_FVector targetPos = Cast<UMyGameInstance>(GetGameInstance())->GetPlayerInfo(
			target)->Position;
		destFVector.X = targetPos.X;
		destFVector.Y = targetPos.Y;
		nextFVector = (destFVector - currentFVector).GetSafeNormal();
	}
	else if (target == _instance->_code)
	{
		Aunreal_5_1_three_cCharacter* player = Cast<Aunreal_5_1_three_cCharacter>(
			GetWorld()->GetFirstPlayerController()->GetCharacter());
		destFVector.X = player->GetActorLocation().X;
		destFVector.Y = player->GetActorLocation().Y;
		nextFVector = (destFVector - currentFVector).GetSafeNormal();
	}

	return nextFVector;
}

void AMyDefaultCharacter::HitDemagePlay(int32 demage)
{
	Aunreal_5_1_three_cGameMode* mode = Cast<
		Aunreal_5_1_three_cGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (mode->UI_Demage)
	{
		UUserWidget* DemageWidget = CreateWidget(GetWorld(), mode->UI_Demage);

		if (DemageWidget)
		{
			FVector2D StartPosition;
			FVector2D EndPosition;
			DemageWidget->AddToViewport();
			UGameplayStatics::ProjectWorldToScreen(UGameplayStatics::GetPlayerController(GetWorld(), 0),
			                                       GetActorLocation(), StartPosition);
			EndPosition = StartPosition;
			EndPosition.Y -= 100.f;

			Cast<UMyDemageWidget>(DemageWidget)->SetDemageText(FText::FromString(FString::FromInt(demage)));
			Cast<UMyDemageWidget>(DemageWidget)->StartDemageAnimation(StartPosition, EndPosition);
		}
	}
}

float AMyDefaultCharacter::GetRotate(FVector& next)
{
	// 보간 문제 캐릭터를 이동할때 바로 회전으로 되기 때문에 바로 반대방향으로 변하기 전에 같은 방식으로 회전이 되어야 될듯하다....
	// 즉 바로 반대 방향으로 될때 180도를 돌아버리는 현상이 있다. 그거 어떻게 하지!!!!
	FVector forward = GetActorForwardVector();

	float dot = FVector::DotProduct(forward, next);
	float AcosAnagle = FMath::Acos(dot);
	float angle = FMath::RadiansToDegrees(AcosAnagle);

	FVector cross = FVector::CrossProduct(forward, next);

	if (cross.Z > 0)
	{
		//right
		return angle;
	}
	if (cross.Z < 0)
	{
		//right
		return angle * -1;
	}
	return 0;
}
