// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGameActionGift.h"
#include "UDGameActionGiftReject.generated.h"

/**
 * Reject version for UUDGiftAction
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGameActionGiftReject : public UUDGameActionGift
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDGameDataTargetAmount::ParameterCount; };
public:
	static const int32 ActionTypeId = 2006;
};