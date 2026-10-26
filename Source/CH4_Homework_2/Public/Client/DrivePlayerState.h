// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTagContainer.h"
#include "DrivePlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, FGameplayTag, int32);

/**
 * 
 */
UCLASS()
class CH4_HOMEWORK_2_API ADrivePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	FOnStatChanged OnStatChanged;

public:
	UPROPERTY(Replicated)
	int32 Uid;

public:
	ADrivePlayerState();

public:
	void AddCoin(int32 InCoin);

	void AddBonusMaxSpeed(int32 InBonusMaxSpeed);

	void SubCoin(int32 OutCoin);

	int32 GetCoin();

	int32 GetBonusMaxSpeed();

	int32 GetStat(FGameplayTag StatTag);

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	void OnRep_BonusMaxSpeed();

	UFUNCTION()
	void OnRep_Coin();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Coin)
	int32 Coin = 0;

	UPROPERTY(ReplicatedUsing = OnRep_BonusMaxSpeed)
	int32 BonusMaxSpeed = 0;
};
