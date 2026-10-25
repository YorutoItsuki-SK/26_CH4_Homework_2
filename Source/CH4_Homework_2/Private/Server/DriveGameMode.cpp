// Fill out your copyright notice in the Description page of Project Settings.


#include "Server/DriveGameMode.h"

#include "Client/DrivePlayerController.h"
#include "Client/DrivePlayerState.h"

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

void ADriveGameMode::StealCoin(AController* Attacker, AController* Victim)
{
	if (!Attacker || !Victim) return;

	ADrivePlayerState* AttackerPS = Attacker->GetPlayerState<ADrivePlayerState>();
	ADrivePlayerState* VictimPS = Victim->GetPlayerState<ADrivePlayerState>();

	if (!AttackerPS || !VictimPS) return;

	int32 VictimCoin = VictimPS->GetCoin();
	if (VictimCoin < 5) return;

	VictimPS->SubCoin(5);
	AttackerPS->AddCoin(5);
}

ERoomState ADriveGameMode::GetRoomState() const
{
	return RoomState;
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

	ChangeRoomState(ERoomState::Waiting);
}

void ADriveGameMode::ChangeRoomState(ERoomState InRoomState)
{
	RoomState = InRoomState;
	OnRoomStateChanged.Broadcast(RoomState);
}
