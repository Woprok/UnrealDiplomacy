// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "UDGaiaActionFactionStratagemActivated.generated.h"

/**
 * Applies faction stratagem use modifier.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGaiaActionFactionStratagemActivated : public UUDGaiaAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDGaiaDataAction::ParameterCount; };
	virtual void SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager) override;
public:
	static const int32 ActionTypeId = 1006;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
};