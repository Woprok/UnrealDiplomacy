// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "UDSystemActionGameEnd.generated.h"

/**
 * Changes the state to post game.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSystemActionGameEnd : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
public:
	static const int32 ActionTypeId = 16;
};