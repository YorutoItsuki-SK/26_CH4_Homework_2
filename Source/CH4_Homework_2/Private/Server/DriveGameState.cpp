// Fill out your copyright notice in the Description page of Project Settings.


#include "Server/DriveGameState.h"

#include "Server/DriveGameMode.h"

#include "Net/UnrealNetwork.h"

void ADriveGameState::SetRoomStateData(FRoomStateData InRoomStateData)
{
	RoomStateData = InRoomStateData;

	DoApplyRoomStateData();
}

FRoomStateData ADriveGameState::GetRoomStateData() const
{
	return RoomStateData;
}

void ADriveGameState::BeginPlay()
{
	Super::BeginPlay();

	if(!HasAuthority()) return;

	ADriveGameMode* DGM = Cast<ADriveGameMode>(GetWorld()->GetAuthGameMode());
	if (!DGM) return;

	DGM->OnRoomStateChanged.AddUObject(this, &ADriveGameState::SetRoomStateData);
}

void ADriveGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, RoomStateData);
}

void ADriveGameState::OnRep_RoomStateData()
{
	DoApplyRoomStateData();
}

void ADriveGameState::DoApplyRoomStateData()
{
	OnRoomStateDataChanged.Broadcast(RoomStateData);
}
