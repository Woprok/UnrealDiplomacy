// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "UDGaiaActionFactionResourceDeficit.generated.h"

/**
 * Penalizes each faction for all resources that are negative.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGaiaActionFactionResourceDeficit : public UUDGaiaAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	//virtual int32 GetParameterCount() const override { return FUDGaiaDataAmount::ParameterCount; };
	virtual void SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager) override;
public:
	static const int32 ActionTypeId = 1004;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
	/** Percantage that is substracted from all resources per negative resource. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	float PenaltyPerResource = 0.1f;
};