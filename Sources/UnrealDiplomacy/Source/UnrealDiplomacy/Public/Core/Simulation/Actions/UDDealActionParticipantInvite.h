// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDDealActionParticipantInvite.generated.h"

/**
 * Invite player to participate, interaction is required for accepting or rejecting.
 * Player that was invinted is then considered participant.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDDealActionParticipantInvite : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	//virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10001;
	static const int32 RequiredParametersCount = 2;
	static FUDDealTargetData ConvertData(FUDActionData& action)
	{
		return FUDDealTargetData(action.ValueParameters);
	}
};