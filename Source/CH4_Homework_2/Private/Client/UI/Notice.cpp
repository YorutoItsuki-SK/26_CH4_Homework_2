// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/UI/Notice.h"

#include "Components/TextBlock.h"

#include "Client/DrivePlayerController.h"

void UNotice::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PCRaw = GetOwningPlayer();
	if (PCRaw) {
		ADrivePlayerController* DPC = Cast<ADrivePlayerController>(PCRaw);
		if (DPC) {
			NoticeHandle = DPC->OnChatRecived.AddUObject(this, &UNotice::RecieveNotice);
		}
	}

	NoticeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
}

void UNotice::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* PCRaw = GetOwningPlayer();
	if (PCRaw) {
		ADrivePlayerController* DPC = Cast<ADrivePlayerController>(PCRaw);
		if (DPC) {
			DPC->OnChatRecived.Remove(NoticeHandle);
		}
	}
}

void UNotice::RecieveNotice(FChatMessage InChatMessage)
{
	if (InChatMessage.ChatMessageType != EChatMessageType::Notice) return;
	
	/*if (UWorld* World = GetWorld()) {
		if (World->GetTimerManager().IsTimerActive(NoticeTimer)) {
			World->GetTimerManager().ClearTimer(NoticeTimer);
		}
		World->GetTimerManager().SetTimer(NoticeTimer, this, &UNotice::DoCollapseNotice, 10.f, false);
	}*/

	NoticeTextBlock->SetText(FText::FromString(InChatMessage.Message));
	NoticeTextBlock->SetVisibility(ESlateVisibility::Visible);
}

void UNotice::DoCollapseNotice()
{
	NoticeTextBlock->SetVisibility(ESlateVisibility::Collapsed);
}
