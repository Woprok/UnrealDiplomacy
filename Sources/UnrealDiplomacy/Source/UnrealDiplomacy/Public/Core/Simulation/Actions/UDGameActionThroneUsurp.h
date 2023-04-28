// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameActionThroneUsurp.generated.h"

/**
 * Enables player to usurp the throne.
 * Unless the usurper is removed from the throne by the end of game, he will be winner.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameActionThroneUsurp : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 10;
	static const int32 RequiredParametersCount = 0;
};