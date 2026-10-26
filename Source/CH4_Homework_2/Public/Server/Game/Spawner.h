// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Server/DriveGameMode.h"
#include "Spawner.generated.h"

class USceneComponent;
class ASpawnAbleItem;

UCLASS()
class CH4_HOMEWORK_2_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawner();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpawnAbleItem> SpawnItemClass;

	UPROPERTY()
	TObjectPtr<ASpawnAbleItem> SpawnItem;

	FDelegateHandle RoomStateHandle;

	UPROPERTY(ReplicatedUsing = OnRep_RoomState)
	ERoomState RoomState = ERoomState::None;

	FTimerHandle SpawnTimer;

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:

	void OnRecive_RoomState(FRoomStateData InRoomStateData);

	void SetRoomState(ERoomState InRoomState);

	UFUNCTION()
	void OnRep_RoomState();

	void DoApplyState();

	void OnItemActivated();
};
