// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Context/DriveStruct.h"
#include "DrivePlayerController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChatRecived, FChatMessage);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnNoticeRecived, FText);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputModeChanged, FInputModeDataBase);

class ADriveGameMode;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class CH4_HOMEWORK_2_API ADrivePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnChatRecived OnChatRecived;
	FOnNoticeRecived OnNoticeRecived;
	FOnInputModeChanged OnInputModeChanged;

public:
	UFUNCTION(Server, Reliable)
	void ServerRPCSendMessage(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCSendMessage(const FChatMessage& InChatMessage);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void OnRep_PlayerState() override;

protected:
	UPROPERTY()
	TObjectPtr<ADriveGameMode> CachedDGM;

	UPROPERTY()
	TObjectPtr<UUserWidget> MainHud = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainHudClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;
};
