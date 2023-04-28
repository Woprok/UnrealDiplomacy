// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionResultClose.generated.h"

/**
 * Moderator is able to close deal, making all actions for it invalid and deak has no result.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionResultClose : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10020;
	static const int32 RequiredParametersCount = 1;
	static FUDDealData ConvertData(FUDActionData& action)
	{
		return FUDDealData(action.ValueParameters);
	}
};