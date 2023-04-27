// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameActionTileTake.generated.h"

/**
 * Change ownership of an tile. Tile will be owned by InvokerPlayer and it will be taken from TargetPlayer.
 * Tile is always owned by someone, either player or world (which is in a certain way also considered player)
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameActionTileTake : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 5004;
	static const int32 RequiredParametersCount = 3;
	static FUDTargetTileData ConvertData(FUDActionData& data)
	{
		return FUDTargetTileData(data.ValueParameters);
	}
};