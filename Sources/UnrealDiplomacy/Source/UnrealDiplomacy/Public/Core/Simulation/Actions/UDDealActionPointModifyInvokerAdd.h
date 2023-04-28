// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointModifyInvokerAdd.generated.h"

UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointModifyInvokerAdd : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10038;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& action)
	{
		return FUDDealPointValueData(action.ValueParameters);
	}
};