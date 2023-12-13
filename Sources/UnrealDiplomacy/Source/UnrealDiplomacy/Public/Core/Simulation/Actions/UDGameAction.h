// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameAction.generated.h"

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGameDataTarget
{
	GENERATED_BODY()
public:
	FUDGameDataTarget() {}
	FUDGameDataTarget(TArray<int32> valueParameters)
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
struct FUDGameDataTargetAmount
{
	GENERATED_BODY()
public:
	FUDGameDataTargetAmount() {}
	FUDGameDataTargetAmount(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		Amount = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Amount = 0;
	static const int32 ParameterCount = 2;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGameDataTargetAction
{
	GENERATED_BODY()
public:
	FUDGameDataTargetAction() {}
	FUDGameDataTargetAction(TArray<int32> valueParameters)
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

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGameDataTargetResourceAmount
{
	GENERATED_BODY()
public:
	FUDGameDataTargetResourceAmount() {}
	FUDGameDataTargetResourceAmount(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		Resource = valueParameters[1];
		Amount = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Resource = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Amount = 0;
	static const int32 ParameterCount = 3;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGameDataTargetTile
{
	GENERATED_BODY()
public:
	FUDGameDataTargetTile() {}
	FUDGameDataTargetTile(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		X = valueParameters[1];
		Y = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
	static const int32 ParameterCount = 3;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGameDataTile
{
	GENERATED_BODY()
public:
	FUDGameDataTile() {}
	FUDGameDataTile(TArray<int32> valueParameters)
	{
		X = valueParameters[0];
		Y = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
	static const int32 ParameterCount = 2;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDGameDataTileAmount
{
	GENERATED_BODY()
public:
	FUDGameDataTileAmount() {}
	FUDGameDataTileAmount(TArray<int32> valueParameters)
	{
		X = valueParameters[0];
		Y = valueParameters[1];
		Amount = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Amount = 0;
	static const int32 ParameterCount = 3;
};

/**
 * Generic ancestor for all Game actions.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDGameAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 2000;
};