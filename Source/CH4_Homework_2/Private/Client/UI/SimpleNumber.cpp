// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/UI/SimpleNumber.h"

#include "Components/TextBlock.h"

#include "Client/DrivePlayerState.h"
#include "Context/DriveGameplayTags.h"

void USimpleNumber::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;
	ADrivePlayerState* DPC = PC->GetPlayerState<ADrivePlayerState>();
	if (!DPC) return;

	TargetHandle = DPC->OnStatChanged.AddUObject(this, &USimpleNumber::ReceiveNewValue);
	ReceiveNewValue(
		TargetTag,
		DPC->GetStat(TargetTag)
	);
}

void USimpleNumber::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* PC = GetOwningPlayer();
	if (!PC) return;
	ADrivePlayerState* DPC = PC->GetPlayerState<ADrivePlayerState>();
	if (!DPC) return;

	DPC->OnStatChanged.Remove(TargetHandle);
}

void USimpleNumber::ReceiveNewValue(FGameplayTag InGameplayTag, int32 NewValue)
{
	if (!(InGameplayTag == TargetTag)) return;

	const FText Result = FText::Format(
		DisplayFormat,
		FText::AsNumber(NewValue)
	);

	TextBox->SetText(Result);
}
