// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DriveStruct.generated.h"

USTRUCT(BlueprintType)
struct FChatMessage 
{
	GENERATED_BODY()

	UPROPERTY()
	FDateTime Time;

	UPROPERTY()
	FString Sender;

	UPROPERTY()
	FString Message;
};

UENUM(BlueprintType)
enum class ERoomState : uint8
{
	None,
	Waiting,
	Playing,
	Ending,
	End
};

USTRUCT(BlueprintType)
struct FRoomStateData
{
	GENERATED_BODY()

	UPROPERTY()
	ERoomState RoomState = ERoomState::None;

	UPROPERTY()
	double StartTime = 0.0;

	UPROPERTY()
	double EndTime = 0.0;
};
