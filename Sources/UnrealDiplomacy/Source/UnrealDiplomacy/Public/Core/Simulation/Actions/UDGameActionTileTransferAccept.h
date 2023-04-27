// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameActionTileTransferAccept.generated.h"

/**
 * Confirm action for UUDTransferTileAction
 * Simple transfer of tile between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameActionTileTransferAccept : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1005;
	static const int32 RequiredParametersCount = 3;
};