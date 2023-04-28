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
	//virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10003;
	static const int32 RequiredParametersCount = 2;
};
