// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointModifyTileValue.generated.h"

// Covers single tile & additional single value actions
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointModifyTileValue : public UObject, public IUDActionInterface
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
	static const int32 ActionTypeId = 10054;
	static const int32 RequiredParametersCount = 5;
	static FUDDealPointTileValueData ConvertData(FUDActionData& data)
	{
		return FUDDealPointTileValueData(data.ValueParameters);
	}
	static FUDTileValueData ConvertBackupData(FUDActionData& data)
	{
		return FUDTileValueData(data.BackupValueParameters);
	}
};