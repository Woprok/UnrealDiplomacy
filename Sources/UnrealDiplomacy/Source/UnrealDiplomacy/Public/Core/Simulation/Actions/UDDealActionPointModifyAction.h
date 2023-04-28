// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointModifyAction.generated.h"

UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointModifyAction : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
	bool IsBackupRequired() override;
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual bool HasContinuations() override { return true; }
	// This action needs to clear all parameters. Clean all parameters is exclusively made for this.
	virtual TArray<FUDActionData> GetContinuations(FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 10034;
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