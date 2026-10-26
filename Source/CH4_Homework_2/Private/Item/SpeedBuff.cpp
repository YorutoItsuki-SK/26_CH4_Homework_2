// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/SpeedBuff.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Net/UnrealNetwork.h"

#include "Client/DrivePlayerState.h"

ASpeedBuff::ASpeedBuff()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Scene);
}

void ASpeedBuff::BeginPlay()
{
	Super::BeginPlay();

	if (Sphere) {
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASpeedBuff::OnOverlapBegin);
	}
}

void ASpeedBuff::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	if (bIsActivated) return;

	if (!OtherActor->ActorHasTag(FName("Player"))) return;

	APawn* PlayerPawn = Cast<APawn>(OtherActor);
	if (!PlayerPawn) return;

	APlayerController* OtherPC = Cast<APlayerController>(PlayerPawn->GetController());
	if (!OtherPC) return;

	ADrivePlayerState* DPS = OtherPC->GetPlayerState<ADrivePlayerState>();
	if (!DPS) return;

	DPS->AddBonusMaxSpeed(SpeedValue);

	DoDeActivate();
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
