// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealAction.h"
#include "UDDealActionPointModify.generated.h"

/**
 * Modifies point.
 */
UCLASS(Abstract)
class UNREALDIPLOMACY_API UUDDealActionPointModify : public UUDDealAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDealDataPoint::ParameterCount; };
public:
	static const int32 ActionTypeId = 3027;
};