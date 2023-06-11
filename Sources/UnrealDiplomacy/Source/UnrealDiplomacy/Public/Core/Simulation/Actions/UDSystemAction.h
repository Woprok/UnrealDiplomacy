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
	static const int32 ParameterCount = 1;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDSystemDataValue
{
	GENERATED_BODY()
public:
	FUDSystemDataValue() {}
	FUDSystemDataValue(TArray<int32> valueParameters)
	{
		Value = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
	static const int32 ParameterCount = 1;
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