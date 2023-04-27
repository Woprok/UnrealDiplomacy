// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionParticipantInviteReject.generated.h"

/**
 * Reject and become unable to become a participant in the deal.
 * Deal can still affect this player, e.g. being target of other action...
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionParticipantInviteReject : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	//virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10003;
	static const int32 RequiredParametersCount = 2;
};
