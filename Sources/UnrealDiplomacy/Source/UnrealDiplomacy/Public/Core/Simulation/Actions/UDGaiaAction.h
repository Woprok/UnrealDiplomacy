// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGaiaAction.generated.h"

/** Transforms action parameters to properly named fields. */
USTRUCT(BlueprintType)
struct FUDGaiaDataAction
{
	GENERATED_BODY()
public:
	FUDGaiaDataAction() {}
	FUDGaiaDataAction(TArray<int32> valueParameters)
	{
		ActionTypeId = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
	static const int32 ParameterCount = 1;
};

/** Transforms action parameters to properly named fields. */
USTRUCT(BlueprintType)
struct FUDGaiaDataTileAction
{
	GENERATED_BODY()
public:
	FUDGaiaDataTileAction() {}
	FUDGaiaDataTileAction(TArray<int32> valueParameters)
	{
		X = valueParameters[0];
		Y = valueParameters[1];
		ActionTypeId = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
	static const int32 ParameterCount = 3;
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