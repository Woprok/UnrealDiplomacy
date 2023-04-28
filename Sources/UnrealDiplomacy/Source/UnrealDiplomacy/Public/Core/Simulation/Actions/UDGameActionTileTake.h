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
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 5004;
	static const int32 RequiredParametersCount = 3;
	static FUDTargetTileData ConvertData(FUDActionData& action)
	{
		return FUDTargetTileData(action.ValueParameters);
	}
};