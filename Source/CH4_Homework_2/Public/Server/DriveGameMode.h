// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Context/DriveStruct.h"
#include "DriveGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnRoomStateChanged, FRoomStateData);

class ADrivePlayerController;

/**
 * 
 */


UCLASS()
class CH4_HOMEWORK_2_API ADriveGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	FOnRoomStateChanged OnRoomStateChanged;

public:
	void BroadcastNewChat(ADrivePlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void StealCoin(AController* Attacker, AController* Victim);

	ERoomState GetRoomState() const;

protected:

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void BeginPlay() override;

protected:
	ERoomState RoomState = ERoomState::None;

	int32 Uid = 0;

	UPROPERTY()
	TArray<TObjectPtr<ADrivePlayerController>> AllPlayers;

	FTimerHandle RoomTimer;

protected:
	void ChangeRoomState(ERoomState InRoomState);

	void StartGame();

	void EndGame();

	void DoResult();
};
