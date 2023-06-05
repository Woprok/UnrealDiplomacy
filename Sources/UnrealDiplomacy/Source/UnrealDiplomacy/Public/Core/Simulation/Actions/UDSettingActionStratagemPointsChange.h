// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSettingAction.h"
#include "UDSettingActionStratagemPointsChange.generated.h"

// Forward Declarations

class UUDFactionState;

/**
 * Change the current amount of points available and reset all player selection.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSettingActionStratagemPointsChange : public UUDSettingAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDSettingDataValue::ParameterCount; };
	bool IsBackupRequired() const override { return true; };
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual bool HasContinuations() const override { return true; }
	// This action needs to clear all selected stratagems for all factions.	
	virtual TArray<FUDActionData> GetContinuations(const FUDActionData& parentAction, TObjectPtr<UUDWorldState> world) const override;
public:
	static const int32 ActionTypeId = 4005;
private:
	TArray<FUDActionData> GetDeselectForAll(TObjectPtr<UUDWorldState> world) const;
	TArray<FUDActionData> GetFactionDeselectAll(TObjectPtr<UUDFactionState> faction) const;
};