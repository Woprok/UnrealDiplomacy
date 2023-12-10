// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "UDGaiaActionResourceYieldFaction.generated.h"

/**
 * Generates income based on parameter for all players.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGaiaActionResourceYieldFaction : public UUDGaiaAction
{
	GENERATED_BODY()
public:
	UUDGaiaActionResourceYieldFaction();
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	TMap<int32, int32> StockpileGains = { };
};