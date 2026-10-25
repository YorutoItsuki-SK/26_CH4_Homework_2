
#include "Item/SpawnAbleItem.h"

#include "Net/UnrealNetwork.h"

ASpawnAbleItem::ASpawnAbleItem()
{
	NetUpdateFrequency = 1;
}

void ASpawnAbleItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsActivated);
}

void ASpawnAbleItem::DoActivate()
{
	if (!HasAuthority()) return;

	bIsActivated = false;
	DoApplayState();
}

void ASpawnAbleItem::DoDeActivate()
{
	if (!HasAuthority()) return;

	bIsActivated = true;
	DoApplayState();
}

void ASpawnAbleItem::OnRep_IsActivated()
{
	DoApplayState();
}

void ASpawnAbleItem::DoApplayState()
{
	SetActorHiddenInGame(bIsActivated);
	SetActorEnableCollision(!bIsActivated);
}

void ASpawnAbleItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

