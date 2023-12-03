// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "UDGaiaActionResourcesAllAdd.generated.h"

/**
 * Generates income based on parameter for all players.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGaiaActionResourcesAllAdd : public UUDGaiaAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	//virtual int32 GetParameterCount() const override { return FUDGaiaDataAmount::ParameterCount; };
	virtual void SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager) override;
public:
	static const int32 ActionTypeId = 1001;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
	UPROPERTY()
	int32 ReputationGain = 5;
	UPROPERTY()
	int32 GoldGain = 25;
	UPROPERTY()
	int32 FoodGain = 100;
	UPROPERTY()
	int32 MaterialsGain = 75;
	UPROPERTY()
	int32 ManpowerGain = 50;
	UPROPERTY()
	int32 LuxuriesGain = 10;
};