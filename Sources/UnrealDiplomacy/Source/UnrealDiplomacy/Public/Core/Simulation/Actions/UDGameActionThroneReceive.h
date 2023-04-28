// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameActionThroneReceive.generated.h"

/**
 * Crown is granted to highest ranked player at the end of the game.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameActionThroneReceive : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 11;
	static const int32 RequiredParametersCount = 1;
	static FUDTargetData ConvertData(FUDActionData& action)
	{
		return FUDTargetData(action.ValueParameters);
	}
};