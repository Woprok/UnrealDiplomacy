// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDecisionAction.h"
#include "UDDecisionActionCreate.generated.h"

/**
 * Creates new decision that needs to be resolved by target.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDecisionActionCreate : public UUDDecisionAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDecisionDataTargetTypeParameters::ParameterCount; };
	virtual FUDActionPresentation GetPresentation() const override;
	virtual bool HasContinuations() const override { return true; }
	// This action can optionally execute it's content without any confirmation.
	virtual TArray<FUDActionData> GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const override;

public:
	static const int32 ActionTypeId = 5001;
};