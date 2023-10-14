// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDecisionAction.h"
#include "UDDecisionActionConsequenceSelect.generated.h"

/**
 * Allows player to define consequence that is invoked on demand rejection.
 * Generally it is normal action that has a special flag.
 * Action can be consequence only if it has no parameter other than faction target.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDecisionActionConsequenceSelect : public UUDDecisionAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDecisionDataAction::ParameterCount; };
	virtual bool IsBackupRequired() const override { return true; };
	virtual void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 2007;
};