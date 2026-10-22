// Fill out your copyright notice in the Description page of Project Settings.


#include "Server/DriveGameMode.h"

#include "Client/DrivePlayerController.h"
#include "Client/DrivePlayerState.h"
#include "Context/DriveStruct.h"

void ADriveGameMode::BroadcastNewChat(ADrivePlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	if (!InChattingPlayerController || InChatMessageString.IsEmpty()) return;
	
	ADrivePlayerState* DPS = InChattingPlayerController->GetPlayerState<ADrivePlayerState>();

	if (!DPS) return;

	FChatMessage NewChatMessage;
	NewChatMessage.Time = FDateTime::Now();
	NewChatMessage.Sender = FString::Printf(TEXT("Player %d"), DPS->Uid);
	NewChatMessage.Message = InChatMessageString;

	for (ADrivePlayerController* DPC : AllPlayers) {
		DPC->ClientRPCSendMessage(NewChatMessage);
	}
}

void ADriveGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (RoomState == ERoomState::Waiting) {
		return;
	}

	ErrorMessage = TEXT("참여가 불가능한 방 입니다");
}

void ADriveGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ADrivePlayerController* DPC = Cast<ADrivePlayerController>(NewPlayer);
	if (!DPC) {
		return;
	}

	ADrivePlayerState* DPS = DPC->GetPlayerState<ADrivePlayerState>();
	if (!DPS) {
		return;
	}

	AllPlayers.Add(DPC);
}

void ADriveGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	ADrivePlayerController* DPC = Cast<ADrivePlayerController>(Exiting);
	if (!DPC) {
		return;
	}

	AllPlayers.Remove(DPC);
}

void ADriveGameMode::BeginPlay()
{
	Super::BeginPlay();

	RoomState = ERoomState::Waiting;
}
