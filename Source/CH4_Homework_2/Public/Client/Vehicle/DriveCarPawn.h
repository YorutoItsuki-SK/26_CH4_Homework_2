// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Context/DriveGameplayTags.h"
#include "DriveCarPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UChaosWheeledVehicleMovementComponent;
struct FInputActionValue;

class ADrivePlayerState;

UCLASS()
class CH4_HOMEWORK_2_API ADriveCarPawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ADriveCarPawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;

	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SteeringAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ThrottleAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> BrakeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> HandbrakeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAroundAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ResetVehicleAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> EnterAction;

	FDelegateHandle MaxSpeedHandle;

	UPROPERTY(EditAnywhere, Category = "Stat|Speed")
	int32 DefaultMaxSpeed = 100;

	int32 FinalMaxSpeed = 100;

	UPROPERTY()
	TObjectPtr<ADrivePlayerState> CachedDPS;

protected:
	virtual void Tick(float Delta) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;

protected:

	/** Called when the brake lights are turned on or off */
	UFUNCTION(BlueprintImplementableEvent, Category = "Vehicle")
	void BrakeLights(bool bBraking);

protected:

	void Steering(const FInputActionValue& Value);

	void Throttle(const FInputActionValue& Value);

	void Brake(const FInputActionValue& Value);

	void StartBrake(const FInputActionValue& Value);
	void StopBrake(const FInputActionValue& Value);

	void StartHandbrake(const FInputActionValue& Value);
	void StopHandbrake(const FInputActionValue& Value);

	void LookAround(const FInputActionValue& Value);

	void ResetVehicle(const FInputActionValue& Value);

	void EnableChat(const FInputActionValue& Value);

	void UpdateMaxSpeed(FGameplayTag StatTag, int32 NewBonusMaxSpeed);

public:

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoSteering(float SteeringValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoThrottle(float ThrottleValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoBrake(float BrakeValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoBrakeStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoBrakeStop();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoHandbrakeStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoHandbrakeStop();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoLookAround(float YawDelta);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoResetVehicle();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoEnableChat();
};
