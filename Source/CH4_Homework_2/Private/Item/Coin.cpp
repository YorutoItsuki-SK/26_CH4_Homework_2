// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Coin.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "Net/UnrealNetwork.h"

#include "Client/DrivePlayerState.h"

ACoin::ACoin()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Scene);
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();

	if (Sphere) {
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnOverlapBegin);
	}
}

void ACoin::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
	DPS->AddCoin(CoinValue);

	DoDeActivate();
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

