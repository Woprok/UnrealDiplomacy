// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemActionTurnEnd.h"
#include "UDSystemActionTurnForceEnd.generated.h"

/**
 * Server ends the turn of player that is targeted by this.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSystemActionTurnForceEnd : public UUDSystemActionTurnEnd
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDSystemDataTarget::ParameterCount; };
public:
	static const int32 ActionTypeId = 8;
};