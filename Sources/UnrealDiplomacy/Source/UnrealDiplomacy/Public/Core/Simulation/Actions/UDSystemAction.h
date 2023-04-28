// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDSystemAction.generated.h"

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDSystemDataTarget
{
	GENERATED_BODY()
public:
	FUDSystemDataTarget() {}
	FUDSystemDataTarget(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	static const int32 ParameterCount = 1;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDSystemDataMapSettings
{
	GENERATED_BODY()
public:
	FUDSystemDataMapSettings() {}
	FUDSystemDataMapSettings(TArray<int32> valueParameters)
	{
		Seed = valueParameters[0];
		SizeX = valueParameters[1];
		SizeY = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 Seed = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 SizeX = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 SizeY = 0;
	UPROPERTY(BlueprintReadOnly)
	static const int32 ParameterCount = 3;
};

/**
 * Generic ancestor for all System actions.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDSystemAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 0;
};