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
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
	bool IsBackupRequired() override;
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 10054;
	static const int32 RequiredParametersCount = 5;
	static FUDDealPointTileValueData ConvertData(FUDActionData& action)
	{
		return FUDDealPointTileValueData(action.ValueParameters);
	}
	static FUDTileValueData ConvertBackupData(FUDActionData& action)
	{
		return FUDTileValueData(action.BackupValueParameters);
	}
};