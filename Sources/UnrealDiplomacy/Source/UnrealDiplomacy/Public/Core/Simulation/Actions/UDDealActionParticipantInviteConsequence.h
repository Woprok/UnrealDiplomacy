// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealActionParticipantInvite.h"
#include "UDDealActionParticipantInviteConsequence.generated.h"

/**
 * Consequence for rejecting invite to a deal.
 * Prevents target from participating in future.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealActionParticipantInviteConsequence : public UUDDealActionParticipantInvite
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDealDataTarget::ParameterCount; };
public:
	static const int32 ActionTypeId = 3004;
};
