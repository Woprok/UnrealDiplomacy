// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointModifyResetParameters.generated.h"

UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointModifyResetParameters : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
	bool RequiresBackup() override;
	void Backup(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
public:
	static const int32 ActionTypeId = 10051;
	static const int32 RequiredParametersCount = 2;
	static FUDDealPointData ConvertData(FUDActionData& data)
	{
		return FUDDealPointData(data.ValueParameters);
	}
};