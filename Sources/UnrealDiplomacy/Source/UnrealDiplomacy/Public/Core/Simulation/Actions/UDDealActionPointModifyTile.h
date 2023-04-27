// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionPointModifyTile.generated.h"

// Covers single tile parameter actions
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionPointModifyTile : public UObject, public IUDActionInterface
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
	static const int32 ActionTypeId = 10053;
	static const int32 RequiredParametersCount = 4;
	static FUDDealPointTileData ConvertData(FUDActionData& data)
	{
		return FUDDealPointTileData(data.ValueParameters);
	}
	static FUDTileData ConvertBackupData(FUDActionData& data)
	{
		return FUDTileData(data.BackupValueParameters);
	}
};
