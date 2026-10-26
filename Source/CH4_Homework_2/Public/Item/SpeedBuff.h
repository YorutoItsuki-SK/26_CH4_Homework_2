// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/SpawnAbleItem.h"
#include "SpeedBuff.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
/**
 * 
 */
UCLASS()
class CH4_HOMEWORK_2_API ASpeedBuff : public ASpawnAbleItem
{
	GENERATED_BODY()

public:
	ASpeedBuff();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	int32 SpeedValue = 10;

protected:
	virtual void BeginPlay() override;

protected:

	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;
};
