// Fill out your copyright notice in the Description page of Project Settings.


#include "Client/Vehicle/DriveCarPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ChaosWheeledVehicleMovementComponent.h"

#include "CH4_Homework_2SportsWheelFront.h"
#include "CH4_Homework_2SportsWheelRear.h"

#include "Client/DrivePlayerState.h"

// Sets default values
ADriveCarPawn::ADriveCarPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(FName("Player"));

	// construct the back camera boom
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Back Spring Arm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 650.0f;
	SpringArm->SocketOffset.Z = 150.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 2.0f;
	SpringArm->CameraLagMaxDistance = 50.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Back Camera"));
	Camera->SetupAttachment(SpringArm);

	// Configure the car mesh
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	// get the Chaos Wheeled movement component
	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	// Note: for faster iteration times, the vehicle setup can be tweaked in the Blueprint instead

	// Set up the chassis
	ChaosVehicleMovement->ChassisHeight = 144.0f;
	ChaosVehicleMovement->DragCoefficient = 0.31f;

	// Set up the wheels
	ChaosVehicleMovement->bLegacyWheelFrictionPosition = true;
	ChaosVehicleMovement->WheelSetups.SetNum(4);

	ChaosVehicleMovement->WheelSetups[0].WheelClass = UCH4_Homework_2SportsWheelFront::StaticClass();
	ChaosVehicleMovement->WheelSetups[0].BoneName = FName("Phys_Wheel_FL");
	ChaosVehicleMovement->WheelSetups[0].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	ChaosVehicleMovement->WheelSetups[1].WheelClass = UCH4_Homework_2SportsWheelFront::StaticClass();
	ChaosVehicleMovement->WheelSetups[1].BoneName = FName("Phys_Wheel_FR");
	ChaosVehicleMovement->WheelSetups[1].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	ChaosVehicleMovement->WheelSetups[2].WheelClass = UCH4_Homework_2SportsWheelRear::StaticClass();
	ChaosVehicleMovement->WheelSetups[2].BoneName = FName("Phys_Wheel_BL");
	ChaosVehicleMovement->WheelSetups[2].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	ChaosVehicleMovement->WheelSetups[3].WheelClass = UCH4_Homework_2SportsWheelRear::StaticClass();
	ChaosVehicleMovement->WheelSetups[3].BoneName = FName("Phys_Wheel_BR");
	ChaosVehicleMovement->WheelSetups[3].AdditionalOffset = FVector(0.0f, 0.0f, 0.0f);

	// Set up the engine
	// NOTE: Check the Blueprint asset for the Torque Curve
	ChaosVehicleMovement->EngineSetup.MaxTorque = 750.0f;
	ChaosVehicleMovement->EngineSetup.MaxRPM = 7000.0f;
	ChaosVehicleMovement->EngineSetup.EngineIdleRPM = 900.0f;
	ChaosVehicleMovement->EngineSetup.EngineBrakeEffect = 0.2f;
	ChaosVehicleMovement->EngineSetup.EngineRevUpMOI = 5.0f;
	ChaosVehicleMovement->EngineSetup.EngineRevDownRate = 600.0f;

	// Set up the transmission
	ChaosVehicleMovement->TransmissionSetup.bUseAutomaticGears = true;
	ChaosVehicleMovement->TransmissionSetup.bUseAutoReverse = true;
	ChaosVehicleMovement->TransmissionSetup.FinalRatio = 2.81f;
	ChaosVehicleMovement->TransmissionSetup.ChangeUpRPM = 6000.0f;
	ChaosVehicleMovement->TransmissionSetup.ChangeDownRPM = 2000.0f;
	ChaosVehicleMovement->TransmissionSetup.GearChangeTime = 0.2f;
	ChaosVehicleMovement->TransmissionSetup.TransmissionEfficiency = 0.9f;

	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios.SetNum(5);
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[0] = 4.25f;
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[1] = 2.52f;
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[2] = 1.66f;
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[3] = 1.22f;
	ChaosVehicleMovement->TransmissionSetup.ForwardGearRatios[4] = 1.0f;

	ChaosVehicleMovement->TransmissionSetup.ReverseGearRatios.SetNum(1);
	ChaosVehicleMovement->TransmissionSetup.ReverseGearRatios[0] = 4.04f;

	// Set up the steering
	// NOTE: Check the Blueprint asset for the Steering Curve
	ChaosVehicleMovement->SteeringSetup.SteeringType = ESteeringType::Ackermann;
	ChaosVehicleMovement->SteeringSetup.AngleRatio = 0.7f;
}

void ADriveCarPawn::Tick(float Delta)
{
	Super::Tick(Delta);

	// add some angular damping if the vehicle is in midair
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);

	// realign the camera yaw to face front
	float CameraYaw = SpringArm->GetRelativeRotation().Yaw;
	CameraYaw = FMath::FInterpTo(CameraYaw, 0.0f, Delta, 1.0f);

	SpringArm->SetRelativeRotation(FRotator(0.0f, CameraYaw, 0.0f));
}

void ADriveCarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EnhancedInputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("ADriveCarPawn::SetupPlayerInputComponent => EnhancedInputComponent Null"));
		return;
	}

	// steering 
	if (SteeringAction) {
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &ADriveCarPawn::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &ADriveCarPawn::Steering);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ADriveCarPawn::SetupPlayerInputComponent => SteeringAction Null"));
	}

	// throttle 
	if (ThrottleAction) {
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &ADriveCarPawn::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &ADriveCarPawn::Throttle);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ADriveCarPawn::SetupPlayerInputComponent => ThrottleAction Null"));
	}

	// break 
	if (BrakeAction) {
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &ADriveCarPawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &ADriveCarPawn::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &ADriveCarPawn::StopBrake);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ADriveCarPawn::SetupPlayerInputComponent => BrakeAction Null"));
	}

	// handbrake 
	if (HandbrakeAction) {
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &ADriveCarPawn::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &ADriveCarPawn::StopHandbrake);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ADriveCarPawn::SetupPlayerInputComponent => HandbrakeAction Null"));
	}

	// look around 
	if (LookAroundAction) {
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &ADriveCarPawn::LookAround);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ADriveCarPawn::SetupPlayerInputComponent => LookAroundAction Null"));
	}

	// reset the vehicle 
	if (ResetVehicleAction) {
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &ADriveCarPawn::ResetVehicle);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ADriveCarPawn::SetupPlayerInputComponent => ResetVehicleAction Null"));
	}

	if (EnterAction) {
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Triggered, this, &ADriveCarPawn::EnableChat);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ADriveCarPawn::SetupPlayerInputComponent => ResetVehicleAction Null"));
	}
}

void ADriveCarPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	CachedDPS = NewController->GetPlayerState<ADrivePlayerState>();

	if (!CachedDPS) return;

	MaxSpeedHandle = CachedDPS->OnStatChanged.AddUObject(this, &ADriveCarPawn::UpdateMaxSpeed);

}

void ADriveCarPawn::UnPossessed()
{
	if (CachedDPS) {
		CachedDPS->OnStatChanged.Remove(MaxSpeedHandle);
	}
	Super::UnPossessed();
}

void ADriveCarPawn::Steering(const FInputActionValue& Value)
{
	DoSteering(Value.Get<float>());
}

void ADriveCarPawn::Throttle(const FInputActionValue& Value)
{
	DoThrottle(Value.Get<float>());
}

void ADriveCarPawn::Brake(const FInputActionValue& Value)
{
	DoBrake(Value.Get<float>());
}

void ADriveCarPawn::StartBrake(const FInputActionValue& Value)
{
	DoBrakeStart();
}

void ADriveCarPawn::StopBrake(const FInputActionValue& Value)
{
	DoBrakeStop();
}

void ADriveCarPawn::StartHandbrake(const FInputActionValue& Value)
{
	DoHandbrakeStart();
}

void ADriveCarPawn::StopHandbrake(const FInputActionValue& Value)
{
	DoHandbrakeStop();
}

void ADriveCarPawn::LookAround(const FInputActionValue& Value)
{
	DoLookAround(Value.Get<float>());
}

void ADriveCarPawn::ResetVehicle(const FInputActionValue& Value)
{
	DoResetVehicle();
}

void ADriveCarPawn::EnableChat(const FInputActionValue& Value)
{
	DoEnableChat();
}

void ADriveCarPawn::UpdateMaxSpeed(FGameplayTag StatTag, int32 NewBonusMaxSpeed)
{
	if (StatTag != Stat_Coin) return;
	FinalMaxSpeed = DefaultMaxSpeed + NewBonusMaxSpeed;
}

void ADriveCarPawn::DoSteering(float SteeringValue)
{
	const float SpeedKmh = FMath::Abs(ChaosVehicleMovement->GetForwardSpeed()) * 0.036f;
	if (SpeedKmh >= FinalMaxSpeed) {
		return;
	}

	// add the input
	ChaosVehicleMovement->SetSteeringInput(SteeringValue);
}

void ADriveCarPawn::DoThrottle(float ThrottleValue)
{
	// add the input
	ChaosVehicleMovement->SetThrottleInput(ThrottleValue);

	// reset the brake input
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void ADriveCarPawn::DoBrake(float BrakeValue)
{
	// add the input
	ChaosVehicleMovement->SetBrakeInput(BrakeValue);

	// reset the throttle input
	ChaosVehicleMovement->SetThrottleInput(0.0f);
}

void ADriveCarPawn::DoBrakeStart()
{
	// call the Blueprint hook for the brake lights
	BrakeLights(true);
}

void ADriveCarPawn::DoBrakeStop()
{
	// call the Blueprint hook for the brake lights
	BrakeLights(false);

	// reset brake input to zero
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void ADriveCarPawn::DoHandbrakeStart()
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(true);

	// call the Blueprint hook for the break lights
	BrakeLights(true);
}

void ADriveCarPawn::DoHandbrakeStop()
{
	// add the input
	ChaosVehicleMovement->SetHandbrakeInput(false);

	// call the Blueprint hook for the break lights
	BrakeLights(false);
}

void ADriveCarPawn::DoLookAround(float YawDelta)
{
	// rotate the spring arm
	SpringArm->AddLocalRotation(FRotator(0.0f, YawDelta, 0.0f));
}

void ADriveCarPawn::DoResetVehicle()
{
	// reset to a location slightly above our current one
	FVector ResetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	// reset to our yaw. Ignore pitch and roll
	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.0f;
	ResetRotation.Roll = 0.0f;

	// teleport the actor to the reset spot and reset physics
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
}

void ADriveCarPawn::DoEnableChat()
{
	FInputModeUIOnly InputModeUIOnly;
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetInputMode(InputModeUIOnly);
}

