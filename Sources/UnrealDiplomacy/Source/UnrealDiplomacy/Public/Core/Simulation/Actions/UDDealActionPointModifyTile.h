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
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
	bool IsBackupRequired() override;
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 10053;
	static const int32 RequiredParametersCount = 4;
	static FUDDealPointTileData ConvertData(FUDActionData& action)
	{
		return FUDDealPointTileData(action.ValueParameters);
	}
	static FUDTileData ConvertBackupData(FUDActionData& action)
	{
		return FUDTileData(action.BackupValueParameters);
	}
};
