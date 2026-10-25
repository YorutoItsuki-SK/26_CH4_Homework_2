// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CountDown.generated.h"

class UTextBlock;

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
	TObjectPtr<UTextBlock> TextBox;

	
};
