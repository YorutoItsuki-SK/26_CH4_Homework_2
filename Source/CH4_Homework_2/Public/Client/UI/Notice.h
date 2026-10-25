// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Context/DriveStruct.h"
#include "Notice.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class CH4_HOMEWORK_2_API UNotice : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	FDelegateHandle NoticeHandle;

	TObjectPtr<UTextBlock> NoticeTextBlock;

protected:
	UFUNCTION()
	void RecieveNotice(FChatMessage InChatMessage);
};
