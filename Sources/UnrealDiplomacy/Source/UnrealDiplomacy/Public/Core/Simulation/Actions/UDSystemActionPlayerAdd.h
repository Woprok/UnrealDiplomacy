// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDSystemActionPlayerAdd.generated.h"

/**
 * Notification for new Player/Ai being part of the game.
 * This allows new player to receive history and create list of all players.
 * Note: Invoker is always the new player, even, if he was added by someone esle AI by server.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDSystemActionPlayerAdd : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual void Revert(FUDActionData& actionData, TObjectPtr<UUDWorldState> targetWorldState) override;
	virtual int32 GetActionTypeId() override { return ActionTypeId; };
	virtual int32 GetRequiredParametersCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1;
	static const int32 RequiredParametersCount = 0;
};