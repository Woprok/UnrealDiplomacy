// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDPawn.h"
#include "InputActionValue.h"
#include "UDSkirmishPawn.generated.h"

// Forward Declarations

class USceneComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * Pawn handles camera movement.
 * This is set to act as RTS camera by default.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishPawn : public AUDPawn
{
	GENERATED_BODY()
public:
	AUDSkirmishPawn();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	/** Binds input */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
protected:
	/** ...and back. */
	void MoveForward(const FInputActionValue& value);
	/** ...and left. */
	void MoveRight(const FInputActionValue& value);
	/** ...and out. */
	void ZoomIn(const FInputActionValue& value);
	/** ...and left. */
	void RotateRight(const FInputActionValue& value);
	/** ...in both directions. e.g. by mouse drag. */
	void RotateDrag(const FInputActionValue& value);
	void EnableRotate(const FInputActionValue& value);
	void CameraBounds();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float MoveSpeed = 20.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float RotateSpeed = 2.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float RotatePitchMin = 10.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float RotatePitchMax = 80.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float MinZoom = 500.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float DefaultZoom = 3000.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float MaxZoom = 4000.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float ZoomSensitivity = 100.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float RotationDegree = 45.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float DefaultPitch = -50.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Settings")
	float ZoomSpeed = 5.0f;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Setup Settings")
	UInputMappingContext* CameraMappingContext;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Setup Settings")
	int32 CameraMappingPriority = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Input Settings")
	UInputAction* MoveForwardAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Input Settings")
	UInputAction* MoveRightAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Input Settings")
	UInputAction* ZoomAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Input Settings")
	UInputAction* RotateAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Input Settings")
	UInputAction* RotateDragAction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera Input Settings")
	UInputAction* RotateEnableAction;

private:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComponent;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
private:
	UPROPERTY()
	FVector TargetLocation;
	UPROPERTY()
	FRotator TargetRotation;
	UPROPERTY()
	float TargetZoom;
	UPROPERTY()
	bool CanRotate;
};