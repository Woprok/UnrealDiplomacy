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
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
	bool IsBackupRequired() override;
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 10052;
	static const int32 RequiredParametersCount = 3;
	static FUDDealPointValueData ConvertData(FUDActionData& action)
	{
		return FUDDealPointValueData(action.ValueParameters);
	}
	static FUDValueData ConvertBackupData(FUDActionData& action)
	{
		return FUDValueData(action.BackupValueParameters);
	}
};