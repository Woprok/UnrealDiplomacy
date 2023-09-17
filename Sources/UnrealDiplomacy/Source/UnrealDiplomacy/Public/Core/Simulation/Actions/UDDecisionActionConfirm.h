// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDecisionAction.h"
#include "UDDecisionActionConfirm.generated.h"

/**
 * Confirms the decision that needed to be resolved by target.
 * Opposite of the UUDDecisionActionDecline.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDecisionActionConfirm : public UUDDecisionAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDecisionDataDecision::ParameterCount; };
	virtual bool HasContinuations() const override { return true; }
	// This action needs to invoke on confirm action.	
	virtual TArray<FUDActionData> GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const override;

public:
	static const int32 ActionTypeId = 5002;
};