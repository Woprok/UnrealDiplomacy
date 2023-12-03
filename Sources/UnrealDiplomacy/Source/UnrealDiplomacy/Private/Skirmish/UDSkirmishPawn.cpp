// Copyright Miroslav Valach

#include "Skirmish/UDSkirmishPawn.h"	
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"

AUDSkirmishPawn::AUDSkirmishPawn()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 2000.0f;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AUDSkirmishPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AUDSkirmishPawn::CameraMoveForward);
		enhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &AUDSkirmishPawn::CameraMoveRight);
		enhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AUDSkirmishPawn::CameraZoomIn);
		enhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AUDSkirmishPawn::CameraRotateRight);
		enhancedInputComponent->BindAction(RotateDragAction, ETriggerEvent::Triggered, this, &AUDSkirmishPawn::CameraRotateDrag);
		enhancedInputComponent->BindAction(RotateEnableAction, ETriggerEvent::Completed, this, &AUDSkirmishPawn::CameraEnableRotate);
	}
}

void AUDSkirmishPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = Cast<APlayerController>(GetController());

	if (!playerController)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPawn: PlayerController is not set for this pawn."));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (!subsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("AUDSkirmishPawn: Input subsystem is not set for this pawn."));
		return;
	}
	subsystem->AddMappingContext(CameraMappingContext, CameraMappingPriority);

	TargetLocation = GetActorLocation();
	TargetZoom = DefaultZoom;
	const FRotator Rotation = SpringArmComponent->GetRelativeRotation();
	TargetRotation = FRotator(Rotation.Pitch + DefaultPitch, Rotation.Yaw, 0.0f);
}

void AUDSkirmishPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraBounds();

	const FVector interpolatedLocation = UKismetMathLibrary::VInterpTo(
		GetActorLocation(), 
		TargetLocation, 
		DeltaTime, 
		MoveSpeed
	);
	SetActorLocation(interpolatedLocation);

	const float interpolatedZoom = UKismetMathLibrary::FInterpTo(
		SpringArmComponent->TargetArmLength, 
		TargetZoom, 
		DeltaTime, 
		ZoomSpeed
	);
	SpringArmComponent->TargetArmLength = interpolatedZoom;

	const FRotator interpolatedRotation = UKismetMathLibrary::RInterpTo(
		SpringArmComponent->GetRelativeRotation(), 
		TargetRotation, 
		DeltaTime, 
		RotateSpeed
	);
	SpringArmComponent->SetRelativeRotation(interpolatedRotation);
}

void AUDSkirmishPawn::CameraMoveForward(const FInputActionValue& value)
{
	float axisValue = value.Get<float>();
	if (axisValue == 0.0f)
	{
		return;
	}

	TargetLocation = SpringArmComponent->GetForwardVector() * axisValue * MoveSpeed + TargetLocation;
}

void AUDSkirmishPawn::CameraMoveRight(const FInputActionValue& value)
{
	float axisValue = value.Get<float>();
	if (axisValue == 0.0f)
	{
		return;
	}

	TargetLocation = SpringArmComponent->GetRightVector() * axisValue * MoveSpeed + TargetLocation;
}

void AUDSkirmishPawn::CameraZoomIn(const FInputActionValue& value)
{
	float axisValue = value.Get<float>();
	if (axisValue == 0.0f)
	{
		return;
	}

	TargetZoom = FMath::Clamp(axisValue * ZoomSensitivity + TargetZoom, MinZoom, MaxZoom);
}

void AUDSkirmishPawn::CameraRotateRight(const FInputActionValue& value)
{
	float axisValue = value.Get<float>();	
	if (axisValue == 0.0f)
	{
		return;
	}
	TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, FMath::Sign(axisValue) * RotationDegree, 0.0f));
}

void AUDSkirmishPawn::CameraRotateDrag(const FInputActionValue& value)
{
	FVector2D axisValue = value.Get<FVector2D>();

	if (axisValue.X != 0.0f && CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(0.0f, axisValue.X, 0.0f));
	}

	if (axisValue.Y != 0.0f && CanRotate)
	{
		TargetRotation = UKismetMathLibrary::ComposeRotators(TargetRotation, FRotator(axisValue.Y, 0.0f, 0.0f));
	}
}

void AUDSkirmishPawn::CameraEnableRotate(const FInputActionValue& value)
{
	CanRotate = !CanRotate;
}

void AUDSkirmishPawn::CameraBounds()
{
	float newPitch = TargetRotation.Pitch;
	if (TargetRotation.Pitch < RotatePitchMax * -1.0f)
	{
		newPitch = RotatePitchMax * -1.0f;
	}
	else if (TargetRotation.Pitch > RotatePitchMin * -1.0f)
	{
		newPitch = RotatePitchMin * -1.0f;
	}

	TargetRotation = FRotator(newPitch, TargetRotation.Yaw, 0.0f);

	TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, 0.0f);

}