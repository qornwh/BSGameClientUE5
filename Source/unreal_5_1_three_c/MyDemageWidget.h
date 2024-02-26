// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyDemageWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_5_1_THREE_C_API UMyDemageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TB_Demage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HitDemageAnim;

	void SetDemageText(FText demage);
	void SetPlay(bool play);

	void SetBindAnim(bool bind);
	bool IsBindAnim();

	void StartDemageAnimation(FVector2D StartPosition, FVector2D EndPosition);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

	FWidgetAnimationDynamicEvent StartDelegate;
	FWidgetAnimationDynamicEvent EndDelegate;
	
	UFUNCTION()
	void AnimationFinished();

private:
	bool _isPlaying = false;
	bool _isBindAnimEnd = false;

	FVector2D _startPosition;
	FVector2D _endPosition;
};
