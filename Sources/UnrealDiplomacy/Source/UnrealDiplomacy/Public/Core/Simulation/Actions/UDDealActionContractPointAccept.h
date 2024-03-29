// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealAction.h"
#include "UDDealActionContractPointAccept.generated.h"

/**
 * Accepts the action that is part of the contract..
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealActionContractPointAccept : public UUDDealAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDealDataContractPoint::ParameterCount; };
public:
	static const int32 ActionTypeId = 3020;
};