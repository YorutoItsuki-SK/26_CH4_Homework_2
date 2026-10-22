// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/UI/ChatInput.h"

#include "Components/EditableTextBox.h"

#include "Client/DrivePlayerController.h"

void UChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (!EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatCommitted)) {
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatCommitted);
	}
}

void UChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatCommitted)) {
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatCommitted);
	}
}

void UChatInput::OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	FInputModeGameOnly InputModeGameOnly;
	PC->SetInputMode(InputModeGameOnly);

	if (!(CommitMethod == ETextCommit::OnEnter)) return;

	APlayerController* OwningPC = GetOwningPlayer();
	if (!OwningPC) return;

	ADrivePlayerController* DPC = Cast<ADrivePlayerController>(OwningPC);
	if (!DPC) return;

	DPC->ServerRPCSendMessage(Text.ToString());

	EditableTextBox_ChatInput->SetText(FText());
}
