// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnAbleItem.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActivated);


UCLASS()
class CH4_HOMEWORK_2_API ASpawnAbleItem : public AActor
{
	GENERATED_BODY()

public:
	ASpawnAbleItem();

public:
	FOnActivated OnActivated;

public:
	virtual void DoActivate();

	virtual void DoDeActivate();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_IsActivated)
	bool bIsActivated = false;

protected:

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void OnRep_IsActivated();

	virtual void DoApplayState();

};
