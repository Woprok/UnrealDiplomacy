// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDSystemActionTurnForceEnd.generated.h"

/**
 * End of turn action that is called as action by server only.
 * Requires TargetParameter that contains the Player that was forced to end the turn.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSystemActionTurnForceEnd : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 5;
	static const int32 RequiredParametersCount = 1;
	static FUDTargetData ConvertData(FUDActionData& data)
	{
		return FUDTargetData(data.ValueParameters);
	}
};