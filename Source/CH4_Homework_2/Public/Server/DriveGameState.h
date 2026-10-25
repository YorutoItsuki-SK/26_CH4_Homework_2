// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Context/DriveStruct.h"
#include "DriveGameState.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoomStateDataChanged, FRoomStateData);

/**
 * 
 */
UCLASS()
class CH4_HOMEWORK_2_API ADriveGameState : public AGameState
{
	GENERATED_BODY()

public:
	FOnRoomStateDataChanged OnRoomStateDataChanged;

public:
	void SetRoomStateData(FRoomStateData InRoomStateData);

	FRoomStateData GetRoomStateData() const;
	
protected:
	UPROPERTY(ReplicatedUsing = OnRep_RoomStateData)
	FRoomStateData RoomStateData;

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_RoomStateData();

	void DoApplyRoomStateData();
};
