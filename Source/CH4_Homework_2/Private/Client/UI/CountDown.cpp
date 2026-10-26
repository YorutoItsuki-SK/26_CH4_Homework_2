// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/UI/CountDown.h"

#include "Components/TextBlock.h"

#include "Server/DriveGameState.h"

void UCountDown::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("UCountDown::NativeConstruct"));

	AGameStateBase* GSRaw = GetWorld()->GetGameState();
	if (GSRaw) {
		CachedDGS = Cast<ADriveGameState>(GSRaw);
		if (CachedDGS) {
			RoomStateDataHandle = CachedDGS->OnRoomStateDataChanged.AddUObject(this, &UCountDown::SetRooumStateData);
			SetRooumStateData(CachedDGS->GetRoomStateData());
		}
	}
}

void UCountDown::NativeDestruct()
{
	if (CachedDGS) {
		CachedDGS->OnRoomStateDataChanged.Remove(RoomStateDataHandle);
	}

	Super::NativeDestruct();
}

void UCountDown::SetRooumStateData(FRoomStateData InRoomStateData)
{
	RoomStateData = InRoomStateData;
	DoApplyCountState();
}

void UCountDown::DoApplyCountState()
{
	if (RoomStateData.RoomState == ERoomState::None) {
		FString EndString = "Player Waitting";
		CounterText->SetText(FText::FromString(EndString));
		return;
	}
	if (RoomStateData.RoomState == ERoomState::Waiting) {
		ResetCountDown();
		return;
	}

	if (RoomStateData.RoomState == ERoomState::Playing) {
		ResetCountDown();
		return;
	}

	ClearCountDown();
	FString EndString = "Match End";
	CounterText->SetText(FText::FromString(EndString));
}

void UCountDown::ClearCountDown()
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (World->GetTimerManager().IsTimerActive(CountDownTimerHandle)) {
		World->GetTimerManager().ClearTimer(CountDownTimerHandle);
	}
}

void UCountDown::ResetCountDown()
{
	ClearCountDown();

	UWorld* World = GetWorld();
	if (!World) return;

	World->GetTimerManager().SetTimer(CountDownTimerHandle, this, &UCountDown::UpdateCountDown, 1.f, true);
}

void UCountDown::UpdateCountDown()
{
	if (!CachedDGS) {
		ClearCountDown();
		return;
	}

	const double Remaining = RoomStateData.EndTime - CachedDGS->GetServerWorldTimeSeconds();
	const int32 Second = FMath::Max(0, Remaining);

	FString StateString = "";
	switch (RoomStateData.RoomState)
	{
	case ERoomState::Waiting:
		StateString = "Waitting";
		break;
	case ERoomState::Playing:
		StateString = "Playing";
		break;
	default:
		break;
	}

	FString CountDownString = FString::Printf(TEXT("%s Time : %d"), *StateString, Second);
	CounterText->SetText(FText::FromString(CountDownString));

	if (Second == 0) {
		ClearCountDown();
	}
}
