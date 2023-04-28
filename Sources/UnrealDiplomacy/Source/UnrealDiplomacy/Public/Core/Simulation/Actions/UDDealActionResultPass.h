// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionResultPass.generated.h"

/**
 * Updates result
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionResultPass : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 100017;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& action)
	{
		return FUDDealData(action.ValueParameters);
	}
};