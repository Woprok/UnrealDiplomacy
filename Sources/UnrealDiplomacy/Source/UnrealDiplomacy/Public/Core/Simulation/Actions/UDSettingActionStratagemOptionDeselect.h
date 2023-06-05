// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSettingAction.h"
#include "UDSettingActionStratagemOptionDeselect.generated.h"

/**
 * Adds specified action as stratagem.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSettingActionStratagemOptionDeselect : public UUDSettingAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDSettingDataValue::ParameterCount; };
public:
	static const int32 ActionTypeId = 4007;
};