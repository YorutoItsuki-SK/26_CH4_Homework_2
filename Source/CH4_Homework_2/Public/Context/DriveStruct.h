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

/**
 * 
 */
class CH4_HOMEWORK_2_API DriveStruct
{
public:
	DriveStruct();
	~DriveStruct();
};
