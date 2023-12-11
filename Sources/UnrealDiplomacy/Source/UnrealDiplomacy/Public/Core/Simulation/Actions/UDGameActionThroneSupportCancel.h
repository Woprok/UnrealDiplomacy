// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGameAction.h"
#include "UDGameActionThroneSupportCancel.generated.h"

/**
 * Cancels throne support.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameActionThroneSupportCancel : public UUDGameAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDGameDataTarget::ParameterCount; };
	virtual void SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager) override;
	virtual FUDActionPresentation GetPresentation() const override;
public:
	static const int32 ActionTypeId = 2017;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
};