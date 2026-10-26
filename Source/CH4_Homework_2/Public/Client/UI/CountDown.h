// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Context/DriveStruct.h"
#include "CountDown.generated.h"

class UTextBlock;
class ADriveGameState;

/**
 * 
 */
UCLASS()
class CH4_HOMEWORK_2_API UCountDown : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CounterText;

	UPROPERTY()
	TObjectPtr<ADriveGameState> CachedDGS;

	FDelegateHandle RoomStateDataHandle;

	FRoomStateData RoomStateData;

	FTimerHandle CountDownTimerHandle;
protected:
	void SetRooumStateData(FRoomStateData InRoomStateData);

	void DoApplyCountState();

	void ClearCountDown();
	
	void ResetCountDown();

	void UpdateCountDown();
};
