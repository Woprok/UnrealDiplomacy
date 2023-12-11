// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "UDGaiaActionTileStratagemActivated.generated.h"

/**
 * Applies tile stratagem use modifier.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGaiaActionTileStratagemActivated : public UUDGaiaAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDGaiaDataTileAction::ParameterCount; };
	virtual void SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager) override;
public:
	static const int32 ActionTypeId = 1005;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
};