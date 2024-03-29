// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealAction.h"
#include "UDDealActionContractExecute.generated.h"

/**
 * Executes all actions in the deal, based on players final believe.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealActionContractExecute : public UUDDealAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDealData::ParameterCount; };
	virtual bool HasContinuations() const override { return true; }
	virtual TArray<FUDActionData> GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const override;
	virtual bool IsBypassingStratagemRequirements() const override { return true; } 
public:
	static const int32 ActionTypeId = 3019;
	TArray<FUDActionData> GetAllContractedActions(TObjectPtr<UUDWorldState> world, int32 dealUniqueId) const;
};