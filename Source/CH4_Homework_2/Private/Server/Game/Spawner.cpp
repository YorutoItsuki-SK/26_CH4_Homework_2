// Fill out your copyright notice in the Description page of Project Settings.


#include "Server/Game/Spawner.h"

#include "Components/SceneComponent.h"
#include "Net/UnrealNetwork.h"

#include "Item/SpawnAbleItem.h"

ASpawner::ASpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(FName("Scene"));
	SetRootComponent(Scene);

}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority()) return;

	if (SpawnItemClass) {
		SpawnItem = GetWorld()->SpawnActor<ASpawnAbleItem>(SpawnItemClass, GetActorLocation(), GetActorRotation());
		if (SpawnItem) {
			SpawnItem->DoDeActivate();
			SpawnItem->OnActivated.AddUObject(this, &ASpawner::OnItemActivated);
		}
	}

	AGameModeBase* GM = GetWorld()->GetAuthGameMode();
	if (GM) {
		ADriveGameMode* DGM = Cast<ADriveGameMode>(GM);
		if (DGM) {
			RoomStateHandle = DGM->OnRoomStateChanged.AddUObject(this, &ASpawner::OnRecive_RoomState);
			SetRoomState(DGM->GetRoomState());
		}
	}
}

void ASpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, RoomState);
}

void ASpawner::OnRecive_RoomState(FRoomStateData InRoomStateData)
{
	SetRoomState(InRoomStateData.RoomState);
}

void ASpawner::SetRoomState(ERoomState InRoomState)
{
	if (!HasAuthority()) return;

	RoomState = InRoomState;

	DoApplyState();
}

void ASpawner::OnRep_RoomState()
{
	DoApplyState();
}

void ASpawner::DoApplyState()
{
	if (RoomState == ERoomState::Playing) {
		if (!SpawnItem) return;

		SpawnItem->DoActivate();
	}
	else {
		if (GetWorldTimerManager().IsTimerActive(SpawnTimer)) {
			GetWorldTimerManager().ClearTimer(SpawnTimer);
		}

		if (SpawnItem) {
			SpawnItem->DoDeActivate();
		}
	}
}

void ASpawner::OnItemActivated()
{
	if (GetWorldTimerManager().IsTimerActive(SpawnTimer)) {
		GetWorldTimerManager().ClearTimer(SpawnTimer);
	}

	GetWorldTimerManager().SetTimer(
		SpawnTimer,
		[this]() {
			if (SpawnItem) {
				SpawnItem->DoActivate();
			}
		},
		2.f,
		false
	);
}

