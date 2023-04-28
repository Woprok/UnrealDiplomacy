// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameActionTileTransfer.generated.h"

/**
 * Simple transfer of tile between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameActionTileTransfer : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1004;
	static const int32 RequiredParametersCount = 3;
	static FUDTargetTileData ConvertData(FUDActionData& action)
	{
		return FUDTargetTileData(action.ValueParameters);
	}
};