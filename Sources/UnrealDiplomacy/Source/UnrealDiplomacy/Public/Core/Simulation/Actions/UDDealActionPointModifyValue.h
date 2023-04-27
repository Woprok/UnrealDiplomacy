// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointModifyValue.generated.h"

// Covers single value parameter actions
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointModifyValue : public UObject, public IUDActionInterface
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
	static const int32 ActionTypeId = 10052;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointValueData(data.ValueParameters);
	}
	static FUDValueData ConvertBackupData(FUDActionData& data)
	{
		return FUDValueData(data.BackupValueParameters);
	}
};