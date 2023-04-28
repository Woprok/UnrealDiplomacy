// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionContractExecute.generated.h"

/**
 * Executes all actions in the deal, based on players final believe.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionContractExecute : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
	virtual bool HasContinuations() override { return true; }
	virtual TArray<FUDActionData> GetContinuations(FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 10055;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& action)
	{
		return FUDDealData(action.ValueParameters);
	}
	static bool AreAllActionsPrepared(TObjectPtr<UUDWorldState> world, int32 dealUniqueId);
};