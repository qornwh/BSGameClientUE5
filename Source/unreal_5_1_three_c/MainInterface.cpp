// Fill out your copyright notice in the Description page of Project Settings.


#include "MainInterface.h"

#include "FPacketHandler.h"
#include "FPacketHeader.h"
#include "unreal_5_1_three_cGameMode.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"
#include "Kismet/GameplayStatics.h"
#include "Protocol/P_PROTOCOL_PAKCET.h"
#include "Serialization/BufferArchive.h"

void UMainInterface::OnEventKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Enter key pressed!"));
}

void UMainInterface::AppendText(FText text)
{
	if (MainChat_SB)
	{
        UTextBlock* TextBox = NewObject<UTextBlock>(MainChat_SB);
		
		if (TextBox)
		{
			UE_LOG(LogTemp, Warning, TEXT("AppendText !!! %s"), *text.ToString());
			MainChat_SB->AddChild(TextBox);
			TextBox->SetText(text);
			TextBox->SetColorAndOpacity(FLinearColor::Black);
			TextBox->SetShadowOffset(FVector2d(5.f, 5.f));
			TextBox->SetShadowColorAndOpacity(FLinearColor::White);
			FSlateFontInfo font = TextBox->GetFont();
			font.Size = 40; 
			TextBox->SetFont(font);
		}
	}
}

void UMainInterface::SendTextMsg()
{
	if (Editor_TB)
	{
		FString text = Editor_TB->GetText().ToString();
		
		P_CHAT_PACKET pkt;
		pkt.Type = 1;
		pkt.TextLen = text.Len() * sizeof(TCHAR);
		pkt.Text = text;

		FBufferArchive Ar;
		int32 size = pkt.Size() + sizeof(FPacketHeader);
		FPacketHandler::MakePacket(pkt, Ar);
		
		Aunreal_5_1_three_cGameMode* mode = Cast<
			Aunreal_5_1_three_cGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		int32 count = mode->GetFScoketActor()->SendMessage(Ar.GetData(), size);
		
		FText str;
		Editor_TB->SetText(str);
	}
}

void UMainInterface::NativeConstruct()
{
	Super::NativeConstruct();

	if (Send_BTN)
	{
		Send_BTN->OnClicked.AddDynamic(this, &UMainInterface::SendTextMsg);
	}
}

FReply UMainInterface::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		OnEventKeyPressed();
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
