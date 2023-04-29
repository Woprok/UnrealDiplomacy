// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealActionPointModify.h"
#include "UDDealActionPointModifyResetParameters.generated.h"

/**
 * Resets current parameters.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealActionPointModifyResetParameters : public UUDDealActionPointModify
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDealDataPoint::ParameterCount; };
	bool IsBackupRequired() const override { return true; };
	void Backup(FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
public:
	static const int32 ActionTypeId = 3032;
};