// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealAction.h"
#include "UDDealActionEndStateDefine.generated.h"

/**
 * Advances the deal state to next in chronological order.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealActionEndStateDefine : public UUDDealAction
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
public:
	static const int32 ActionTypeId = 3017;
};