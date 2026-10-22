// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "SimpleNumber.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class CH4_HOMEWORK_2_API USimpleNumber : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBox;

	UPROPERTY(EditAnywhere, Category = "UI")
	FGameplayTag TargetTag;

	UPROPERTY(EditAnywhere, Category = "UI")
	FText DisplayFormat;

	FDelegateHandle TargetHandle;

protected:
	void ReceiveNewValue(FGameplayTag InGameplayTag, int32 NewValue);
};
