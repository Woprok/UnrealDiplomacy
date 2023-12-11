// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "UDGaiaActionStratagemUseReset.generated.h"

/**
 * Resets all stratagem use modifiers.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGaiaActionStratagemUseReset : public UUDGaiaAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	//virtual int32 GetParameterCount() const override { return FUDGaiaDataAmount::ParameterCount; };
	virtual void SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager) override;
public:
	static const int32 ActionTypeId = 1003;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
};