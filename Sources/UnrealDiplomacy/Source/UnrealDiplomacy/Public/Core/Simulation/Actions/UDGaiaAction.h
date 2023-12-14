// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGaiaAction.generated.h"

/** Transforms action parameters to properly named fields. */
USTRUCT(BlueprintType)
struct FUDGaiaDataTarget
{
	GENERATED_BODY()
public:
	FUDGaiaDataTarget() {}
	FUDGaiaDataTarget(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	static const int32 ParameterCount = 1;
};

/** Transforms action parameters to properly named fields. */
USTRUCT(BlueprintType)
struct FUDGaiaDataTargetAction
{
	GENERATED_BODY()
public:
	FUDGaiaDataTargetAction() {}
	FUDGaiaDataTargetAction(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		ActionTypeId = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
	static const int32 ParameterCount = 2;
};

/** Transforms action parameters to properly named fields. */
USTRUCT(BlueprintType)
struct FUDGaiaDataTargetTileAction
{
	GENERATED_BODY()
public:
	FUDGaiaDataTargetTileAction() {}
	FUDGaiaDataTargetTileAction(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		X = valueParameters[1];
		Y = valueParameters[2];
		ActionTypeId = valueParameters[3];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
	static const int32 ParameterCount = 4;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGaiaDataAmount
{
	GENERATED_BODY()
public:
	FUDGaiaDataAmount() {}
	FUDGaiaDataAmount(TArray<int32> valueParameters)
	{
		Amount = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 Amount = 0;
	static const int32 ParameterCount = 1;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGaiaDataResourceAmount
{
	GENERATED_BODY()
public:
	FUDGaiaDataResourceAmount() {}
	FUDGaiaDataResourceAmount(TArray<int32> valueParameters)
	{
		ResourceId = valueParameters[0];
		Amount = valueParameters[1];
	}
	FUDGaiaDataResourceAmount(int32 resourceId, int32 resourceAmount)
	{
		ResourceId = resourceId;
		Amount = resourceAmount;
	}
	UPROPERTY(BlueprintReadOnly)
	int32 ResourceId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Amount = 0;
	static const int32 ParameterCount = 2;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGaiaDataTargetResourceAmount
{
	GENERATED_BODY()
public:
	FUDGaiaDataTargetResourceAmount() {}
	FUDGaiaDataTargetResourceAmount(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		for (int32 i = 1; i < valueParameters.Num(); i += 2)
		{
			if (i + 1 >= valueParameters.Num())
				break;
			Costs.Add(FUDGaiaDataResourceAmount(valueParameters[i], valueParameters[i+1]));
		}
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDGaiaDataResourceAmount> Costs = { };
	static const int32 ParameterCount = 3;
};

/**
 * Generic ancestor for all Gaia actions.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDGaiaAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 1000;
};