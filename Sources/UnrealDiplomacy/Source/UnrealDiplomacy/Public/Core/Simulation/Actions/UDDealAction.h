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