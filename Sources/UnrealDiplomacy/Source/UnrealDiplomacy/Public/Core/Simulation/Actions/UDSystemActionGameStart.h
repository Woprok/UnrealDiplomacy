// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDSystemActionGameStart.generated.h"

/**
 * Start of game action that is called as action only once.
 * This is only due to fact that we are otherwise using only end turn action.
 * Which is for most purposes sufficient, as any player can check if they are currently playing
 * by observing their state. First player does not know this, so Gaia which takes the first turn
 * is awaiting for this to end her turn.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSystemActionGameStart : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static 	const int32 ActionTypeId = 2;
	static const int32 RequiredParametersCount = 0;
};
