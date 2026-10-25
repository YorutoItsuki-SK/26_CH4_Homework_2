// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/UI/ChatLog.h"

#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"

#include "Client/DrivePlayerController.h"

void UChatLog::AddChatToLog(const FString& Chat)
{
	if (!ChatLogBox) return;
	UTextBlock* NewChat = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

	if (!NewChat) return;
	NewChat->SetText(FText::FromString(Chat));
	NewChat->SetAutoWrapText(true);
	NewChat->SetWrapTextAt(0.f);
	NewChat->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);

	ChatLogBox->AddChildToVerticalBox(NewChat);

	ChatScrollBox->ScrollToEnd();
}

void UChatLog::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PCRaw = GetOwningPlayer();
	if (PCRaw) {
		ADrivePlayerController* DPC = Cast<ADrivePlayerController>(PCRaw);
		if (DPC) {
			ChatHandle = DPC->OnChatRecived.AddUObject(this, &UChatLog::ReciveChat);
		}
	}
}

void UChatLog::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* PCRaw = GetOwningPlayer();
	if (PCRaw) {
		ADrivePlayerController* DPC = Cast<ADrivePlayerController>(PCRaw);
		if (DPC) {
			DPC->OnChatRecived.Remove(ChatHandle);
		}
	}
}

void UChatLog::ReciveChat(FChatMessage InChat)
{
	if (InChat.ChatMessageType != EChatMessageType::Normal) return;
	FDateTime publishedDateTime = InChat.Time;
	FString publishedTime = publishedDateTime.ToString(TEXT("%y:%m:%d %H:%M"));
	FString CombinedMessage = TEXT("[") + publishedTime + TEXT("] ") + InChat.Sender + TEXT(" : ") + InChat.Message;
	AddChatToLog(CombinedMessage);
}
