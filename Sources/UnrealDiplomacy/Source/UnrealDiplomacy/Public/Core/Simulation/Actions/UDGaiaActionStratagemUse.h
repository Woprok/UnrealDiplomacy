// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "UDGaiaActionStratagemUse.generated.h"

/**
 * Applies stratagem use modifier.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGaiaActionStratagemUse : public UUDGaiaAction
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