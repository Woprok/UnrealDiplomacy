// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGameAction.h"
#include "UDGameActionThroneAbdicate.generated.h"

/**
 * Current usurper can abdicate or be forced to abdicate via deal.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameActionThroneAbdicate : public UUDGameAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual FUDActionPresentation GetPresentation() const override;
public:
	static const int32 ActionTypeId = 2002;
};
