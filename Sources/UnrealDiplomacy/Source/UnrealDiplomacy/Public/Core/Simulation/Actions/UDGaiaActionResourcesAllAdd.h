// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGaiaActionResourcesAllAdd.generated.h"

/**
 * Generates income for all players.
 * Called only on GAIA turn.
 * Requires ValueParameter to determine amount to generate for all players.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGaiaActionResourcesAllAdd : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 100;
	static const int32 RequiredParametersCount = 1;
	static FUDValueData ConvertData(FUDActionData& data)
	{
		return FUDValueData(data.ValueParameters);
	}
};