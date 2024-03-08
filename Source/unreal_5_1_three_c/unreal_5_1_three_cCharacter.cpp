// Copyright Epic Games, Inc. All Rights Reserved.

#include "unreal_5_1_three_cCharacter.h"

#include "AMyDefaultCharatorAnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FPacketHandler.h"
#include "FPacketHeader.h"
#include "MagicBall.h"
#include "MyDefaultCharacter.h"
#include "MyGameInstance.h"
#include "MyUserWidget.h"
#include "unreal_5_1_three_cGameMode.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Protocol/P_PROTOCOL_PAKCET.h"
#include "Serialization/BufferArchive.h"
#include "Struct/PlayerInfo.h"

//////////////////////////////////////////////////////////////////////////
// Aunreal_5_1_three_cCharacter

Aunreal_5_1_three_cCharacter::Aunreal_5_1_three_cCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("DefaultCharactor"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Attack
		(TEXT("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Attack.IA_Attack'"));
	if (IA_Attack.Succeeded())
		AttackAction = IA_Attack.Object;

	// ui
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ThirdPerson/Blueprints/WBP_NAME.WBP_NAME_C'"));
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

void Aunreal_5_1_three_cCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	AnimInstance = Cast<UAMyDefaultCharatorAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(AnimInstance, &UAMyDefaultCharatorAnimInstance::OnAttackMontageEnded);
		// 공격 디버깅 어디에 찍혔는지
		AnimInstance->OnAttackHit.AddUObject(this, &Aunreal_5_1_three_cCharacter::AttackCheck);
	}

	MovePlayer.BindUObject(this, &Aunreal_5_1_three_cCharacter::MovePlayerData);
	if (NameUi)
	{
		UMyGameInstance* instance = Cast<UMyGameInstance>(GetGameInstance());

		auto NameWidget = Cast<UMyUserWidget>(NameUi->GetUserWidgetObject());
		if (instance)
		{
			FString name = instance->_name;
			NameWidget->SetText(FText::FromString(name));
		}
	}
}

void Aunreal_5_1_three_cCharacter::MovePlayerData()
{
	FVector currentPos = GetActorLocation();

	if (!currentPos.Equals(PrePosition, 0.3))
	{
		SendMoveMessage();
		PrePosition = currentPos;
		IsMoveTickCount = 0;
		IsMoveStart = true;
	}
	else
	{
		if (IsMoveStart && IsMoveTickCount)
		{
			SendMoveMessage();
			IsMoveStart = false;
		}
		if (IsMoveTickCount > 10)
		{
			SendMoveMessage();
			IsMoveTickCount = 0;
		}
		IsMoveTickCount++;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void Aunreal_5_1_three_cCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this,
		                                   &Aunreal_5_1_three_cCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this,
		                                   &Aunreal_5_1_three_cCharacter::Look);

		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this,
		                                   &Aunreal_5_1_three_cCharacter::Attack);
	}

	PlayerInputComponent->BindAction("ShowMouseAction", IE_Pressed, this,
	                                 FInputActionHandlerSignature::TMethodPtr<Aunreal_5_1_three_cCharacter>(
		                                 &Aunreal_5_1_three_cCharacter::ShowMouse));
}

void Aunreal_5_1_three_cCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 500.f;
	float AttackRadius = 50.f;

	// 멀티 : GetWorld()->SweepMultiByChannel()
	
	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	// FVector Vec = GetActorForwardVector() * AttackRange;
	// FVector Center = GetActorLocation() + Vec * 0.5f;
	// float HalfHeight = AttackRange * 0.5f + AttackRadius;
	// FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	// FColor DrawColor;
	// if (bResult)
	// 	DrawColor = FColor::Green;
	// else
	// 	DrawColor = FColor::Red;
	// DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
	// 	Rotation, DrawColor, false, 2.f);

	if (bResult && HitResult.GetActor()->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.GetActor()->GetName());

		AMyDefaultCharacter* monster  = Cast<AMyDefaultCharacter>(HitResult.GetActor());

		if (monster && monster->GetUnitInfo()->Code < 0)
		{
			SendHitMessage(monster->GetUnitInfo()->Code);
			monster->GetUnitInfo()->Hit = 3;
		}
	
		// FDamageEvent DamageEvent;
		// HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
	}
}

void Aunreal_5_1_three_cCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !AnimInstance->GetAttacking())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 이동 입력값을 하나의 벡터로 조합합니다.
		FVector DesiredMovementInput = ForwardDirection * MovementVector.Y + RightDirection * MovementVector.X;

		// 이동 입력값을 정규화합니다 (길이를 1로 만듭니다).
		DesiredMovementInput.Normalize();

		AddMovementInput(DesiredMovementInput);
	}
}

void Aunreal_5_1_three_cCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void Aunreal_5_1_three_cCharacter::ShowMouse(const FInputActionValue& Value)
{
	// 일단 마우스 커서 보이도록 임시 처리
	auto control = GetWorld()->GetFirstPlayerController();
	if (control->ShouldShowMouseCursor())
	{
		control->SetShowMouseCursor(false);
	}
	else
	{
		control->SetShowMouseCursor(true);
	}
}

void Aunreal_5_1_three_cCharacter::Attack(const FInputActionValue& Value)
{
	if (AnimInstance->GetAttacking())
		return;
	
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
	
	AnimInstance->PlayAttackMontage();
	AnimInstance->JumpToAttack(AttackIndex + 1);
	SendSkillMessage(AttackIndex + 1);
	AttackIndex = (AttackIndex + 1) % AttackSize;
	AnimInstance->SetAttacking(true);
}

void Aunreal_5_1_three_cCharacter::SendMoveMessage()
{
	FVector currentPos = GetActorLocation();
	FRotator currentRot = GetActorRotation();

	// 메시지 보내기
	P_MOVE_PACKET pkt{
		static_cast<int32>(currentPos.X), static_cast<int32>(currentPos.Y), static_cast<int32>(currentPos.Z),
		static_cast<float>(currentRot.Yaw)
	};

	FBufferArchive Ar;
	int32 size = pkt.Size() + sizeof(FPacketHeader);
	FPacketHandler::MakePacket(pkt, Ar);

	Aunreal_5_1_three_cGameMode* mode = Cast<
		Aunreal_5_1_three_cGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	int32 count = mode->GetFScoketActor()->SendMessage(Ar.GetData(), size);
}

void Aunreal_5_1_three_cCharacter::SendSkillMessage(int32 SkillCode)
{
	// 메시지 보내기
	P_SKILL_PACKET pkt{SkillCode};

	FBufferArchive Ar;
	int32 size = pkt.Size() + sizeof(FPacketHeader);
	FPacketHandler::MakePacket(pkt, Ar);

	Aunreal_5_1_three_cGameMode* mode = Cast<
		Aunreal_5_1_three_cGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	int32 count = mode->GetFScoketActor()->SendMessage(Ar.GetData(), size);
}

void Aunreal_5_1_three_cCharacter::SendHitMessage(int32 Code)
{
	// 메시지 보내기
	P_HIT_PACKET pkt{Code, 0};

	FBufferArchive Ar;
	int32 size = pkt.Size() + sizeof(FPacketHeader);
	FPacketHandler::MakePacket(pkt, Ar);

	Aunreal_5_1_three_cGameMode* mode = Cast<
		Aunreal_5_1_three_cGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	int32 count = mode->GetFScoketActor()->SendMessage(Ar.GetData(), size);
}
