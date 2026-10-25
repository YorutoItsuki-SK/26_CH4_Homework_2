// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/DrivePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"

#include "Server/DriveGameMode.h"

void ADrivePlayerController::ServerRPCSendMessage_Implementation(const FString& InChatMessageString)
{
	if (!CachedDGM) {
		CachedDGM = Cast<ADriveGameMode>(GetWorld()->GetAuthGameMode());
		if (!CachedDGM) {
			return;
		}
	}

	CachedDGM->BroadcastNewChat(this, InChatMessageString);
}

void ADrivePlayerController::ClientRPCSendMessage_Implementation(const FChatMessage& InChatMessage)
{
	OnChatRecived.Broadcast(InChatMessage);
}

void ADrivePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADrivePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!IsLocalPlayerController()) {
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!Subsystem) {
		UE_LOG(LogTemp, Warning, TEXT("ADrivePlayerController::SetupInputComponent => Subsystem is null"));
		return;
	}

	for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
	{
		Subsystem->AddMappingContext(CurrentContext, 0);
	}
}

void ADrivePlayerController::OnRep_PlayerState()
{
	if (!IsLocalController()) {
		return;
	}

	if (MainHudClass) {
		MainHud = CreateWidget<UUserWidget>(this, MainHudClass);
		MainHud->AddToViewport();
	}
}
