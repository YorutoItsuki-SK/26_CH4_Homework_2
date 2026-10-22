// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/DrivePlayerState.h"

#include "Net/UnrealNetwork.h"

#include "Context/DriveGameplayTags.h"

ADrivePlayerState::ADrivePlayerState()
{
	bReplicates = true;
}

void ADrivePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Uid);
	DOREPLIFETIME_CONDITION(ThisClass, Coin, COND_OwnerOnly);
}

void ADrivePlayerState::SetBounusMaxSpeed(int32 NewBonusMaxSpeed)
{
	BonusMaxSpeed = FMath::Max(0, NewBonusMaxSpeed);

	OnStatChanged.Broadcast(Stat_BonusMaxSpeed, BonusMaxSpeed);
}

void ADrivePlayerState::OnRep_BonusMaxSpeed()
{
	OnStatChanged.Broadcast(Stat_BonusMaxSpeed, BonusMaxSpeed);
}

void ADrivePlayerState::OnRep_Coin()
{
	OnStatChanged.Broadcast(Stat_Coin, Coin);
}

void ADrivePlayerState::AddCoin(int32 InCoin)
{
	if (!HasAuthority()) return;

	if (InCoin <= 0) return;

	Coin += InCoin;

	OnStatChanged.Broadcast(Stat_Coin, Coin);
}

void ADrivePlayerState::SubCoin(int32 OutCoin)
{
	if (!HasAuthority()) return;

	if (OutCoin <= 0) return;

	Coin -= OutCoin;

	OnStatChanged.Broadcast(Stat_Coin, Coin);
}

int32 ADrivePlayerState::GetCoin()
{
	return Coin;
}

int32 ADrivePlayerState::GetBonusMaxSpeed()
{
	return BonusMaxSpeed;
}

int32 ADrivePlayerState::GetStat(FGameplayTag StatTag)
{
	if (StatTag == Stat_Coin) {
		return Coin;
	}
	else {
		return BonusMaxSpeed;
	}
}
