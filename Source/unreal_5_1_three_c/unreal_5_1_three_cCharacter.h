// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "unreal_5_1_three_cCharacter.generated.h"

DECLARE_DELEGATE(MovePlayerDELEGATE)

UCLASS(config=Game)
class Aunreal_5_1_three_cCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Mouse Show Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShowMouseAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY()
	class UAMyDefaultCharatorAnimInstance* AnimInstance;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AttackCheck();
	// To add mapping context
	virtual void BeginPlay() override;

public:
	Aunreal_5_1_three_cCharacter();

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for OnOff MouseView */
	void ShowMouse(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	/** Returns CameraBoom suHbobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	MovePlayerDELEGATE MovePlayer;

	void MovePlayerData();

	void SendMoveMessage();
	void SendSkillMessage(int32 SkillCode);
	void SendHitMessage(int32 Code);

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* NameUi;

private:
	FVector PrePosition = FVector::ZeroVector;
	int32 IsMoveTickCount = 0;
	bool IsMoveStart = false;
	int32 AttackIndex = 0;
	int32 AttackSize = 2;
};
