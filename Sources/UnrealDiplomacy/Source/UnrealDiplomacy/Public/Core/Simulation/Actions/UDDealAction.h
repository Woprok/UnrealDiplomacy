// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealAction.generated.h"

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealData
{
	GENERATED_BODY()
public:
	FUDDealData() {}
	FUDDealData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	static const int32 ParameterCount = 1;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealDataPoint
{
	GENERATED_BODY()
public:
	FUDDealDataPoint() {}
	FUDDealDataPoint(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		PointId = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointId = 0;
	static const int32 ParameterCount = 2;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealDataPointTarget
{
	GENERATED_BODY()
public:
	FUDDealDataPointTarget() {}
	FUDDealDataPointTarget(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		PointId = valueParameters[1];
		TargetId = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	static const int32 ParameterCount = 3;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealDataPointParameters
{
	GENERATED_BODY()
public:
	FUDDealDataPointParameters() {}
	FUDDealDataPointParameters(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		PointId = valueParameters[1];
		for (int32 i = 2; i < valueParameters.Num(); i++)
		{
			Parameters.Add(valueParameters[i]);
		}
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointId = 0;
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> Parameters = { };
	static const int32 ParameterCount = 2;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealDataPointValue
{
	GENERATED_BODY()
public:
	FUDDealDataPointValue() {}
	FUDDealDataPointValue(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		PointId = valueParameters[1];
		Value = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
	static const int32 ParameterCount = 3;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealDataPointTile
{
	GENERATED_BODY()
public:
	FUDDealDataPointTile() {}
	FUDDealDataPointTile(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		PointId = valueParameters[1];
		X = valueParameters[2];
		Y = valueParameters[3];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
	static const int32 ParameterCount = 4;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealDataPointTileValue
{
	GENERATED_BODY()
public:
	FUDDealDataPointTileValue() {}
	FUDDealDataPointTileValue(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		PointId = valueParameters[1];
		X = valueParameters[2];
		Y = valueParameters[3];
		Value = valueParameters[4];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
	static const int32 ParameterCount = 5;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealDataContractPoint
{
	GENERATED_BODY()
public:
	FUDDealDataContractPoint() {}
	FUDDealDataContractPoint(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		ContractPointId = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 ContractPointId = 0;
	static const int32 ParameterCount = 2;
};

/**
 * Transforms action parameters to properly named fields.
 */
USTRUCT(BlueprintType)
struct FUDDealDataTarget
{
	GENERATED_BODY()
public:
	FUDDealDataTarget() {}
	FUDDealDataTarget(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		TargetId = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	static const int32 ParameterCount = 2;
};

UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 3000;
};