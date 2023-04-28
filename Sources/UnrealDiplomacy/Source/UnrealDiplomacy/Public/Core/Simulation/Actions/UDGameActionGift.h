// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/UDActionInterface.h"
#include "UDGameActionGift.generated.h"

/**
 * Simple gift of resources between two players, that must be accepted.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDGameActionGift : public UObject, public IUDActionInterface
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() override { return ActionTypeId; };
	virtual int32 GetParameterCount() override { return RequiredParametersCount; };
public:
	static const int32 ActionTypeId = 1001;
	static const int32 RequiredParametersCount = 2;
	static FUDTargetValueData ConvertData(FUDActionData& action)
	{
		return FUDTargetValueData(action.ValueParameters);
	}
};