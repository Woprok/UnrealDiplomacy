// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGameActionTileTransfer.h"
#include "UDGameActionTileTransferReject.generated.h"

/**
 * Reject version for UUDGameActionTileTransfer.
 * TODO DELETE
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameActionTileTransferReject : public UUDGameActionTileTransfer
{
	GENERATED_BODY()
public:
	//virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	//virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	//virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDGameDataTargetTile::ParameterCount; };
public:
	static const int32 ActionTypeId = 2010;
};



/**
 * Transforms action parameters to properly named fields.
 *//*/
USTRUCT(BlueprintType)
struct FUDGameDataTargetParameters
{
	GENERATED_BODY()
public:
	FUDGameDataTargetParameters() {}
	FUDGameDataTargetParameters(TArray<int32> valueParameters)
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
};*/