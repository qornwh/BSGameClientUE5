// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDemageWidget.h"

#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UMyDemageWidget::SetDemageText(FText demage)
{
	TB_Demage->SetText(demage);
}

void UMyDemageWidget::SetPlay(bool play)
{
	_isPlaying = play;
}

void UMyDemageWidget::SetBindAnim(bool bind)
{
	_isBindAnimEnd = bind;
}

bool UMyDemageWidget::IsBindAnim()
{
	return _isBindAnimEnd;
}

void UMyDemageWidget::StartDemageAnimation(FVector2D StartPosition, FVector2D EndPosition)
{
	SetPositionInViewport(StartPosition);
	PlayAnimation(HitDemageAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	// 애니메이션 속성 설정
	_startPosition = StartPosition;
	_endPosition = EndPosition;
	
	// 보간 애니메이션 적용
	PlayAnimation(HitDemageAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	EndDelegate.BindDynamic(this, &UMyDemageWidget::AnimationFinished);
	BindToAnimationFinished(HitDemageAnim, EndDelegate);
}

void UMyDemageWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	_startPosition = FMath::Vector2DInterpTo(_startPosition, _endPosition, InDeltaTime, 2.0f);
	
	SetPositionInViewport(_startPosition);
}

void UMyDemageWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMyDemageWidget::AnimationFinished()
{
	RemoveFromParent();
}
