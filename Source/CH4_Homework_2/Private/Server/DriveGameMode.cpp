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
}

void ADriveGameMode::ChangeRoomState(ERoomState InRoomState)
{
	RoomState = InRoomState;
	FRoomStateData NewRoomStateData;
	NewRoomStateData.RoomState = InRoomState; 

	if (InRoomState == ERoomState::Waiting) {
		float WaittingTime = 20.f;
		NewRoomStateData.StartTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		NewRoomStateData.EndTime = NewRoomStateData.StartTime + WaittingTime;
		GetWorldTimerManager().SetTimer(RoomTimer, this, &ADriveGameMode::StartGame, WaittingTime, false);
	}

	if (InRoomState == ERoomState::Playing) {
		float MatchTime = 60.f;
		NewRoomStateData.StartTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
		NewRoomStateData.EndTime = NewRoomStateData.StartTime + MatchTime;
		GetWorldTimerManager().SetTimer(RoomTimer, this, &ADriveGameMode::EndGame, MatchTime, false);
	}
	OnRoomStateChanged.Broadcast(NewRoomStateData);
}

void ADriveGameMode::StartGame()
{
	ChangeRoomState(ERoomState::Playing);
}

void ADriveGameMode::EndGame()
{
	ChangeRoomState(ERoomState::End);
	DoResult();
}

void ADriveGameMode::DoResult()
{
	int32 MaxCoinScore = 0;

	for (ADrivePlayerController* DPC : AllPlayers) {
		ADrivePlayerState* DPS = DPC->GetPlayerState<ADrivePlayerState>();
		if (!DPS) continue;

		MaxCoinScore = FMath::Max(MaxCoinScore, DPS->GetCoin());
	}

	for (ADrivePlayerController* DPC : AllPlayers) {
		ADrivePlayerState* DPS = DPC->GetPlayerState<ADrivePlayerState>();
		if (!DPS) continue;

		FChatMessage ResultMessage;
		ResultMessage.ChatMessageType = EChatMessageType::Notice;
		ResultMessage.Sender = "Server";
		if (DPS->GetCoin() == MaxCoinScore) {
			ResultMessage.Message = "Victory";
		}
		else {
			ResultMessage.Message = "Defeat";
		}
		DPC->ClientRPCSendMessage(ResultMessage);
	}
}
