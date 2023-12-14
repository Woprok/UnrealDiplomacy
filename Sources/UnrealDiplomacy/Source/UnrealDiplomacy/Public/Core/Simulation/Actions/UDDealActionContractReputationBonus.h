// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDDealAction.h"
#include "UDDealActionContractReputationBonus.generated.h"

/**
 * Evaluates reputation of this deal before the all mighty authority.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDDealActionContractReputationBonus : public UUDDealAction
{
	GENERATED_BODY()
public:
	virtual bool CanExecute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) const override;
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual int32 GetParameterCount() const override { return FUDDealData::ParameterCount; };
	virtual void SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager) override;
public:
	static const int32 ActionTypeId = 3038;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
	/** Reputation for the deal, being successful. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	int32 BaseDealReputation = 50;
	/** Each time a point of deal is broken, final reward is lowered by certain perc, up to 100%. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float PenaltyPerFailedFinalPoint = 0.07f;
	/** Each time a point of deal is broken, final reward is lowered by certain perc, up to 100%. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float PenaltyPerChangedFinalPoint = 0.01f;
	/** Each Invoker is considered Active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float BonusPerDealActiveParticipant = 0.02f;
	/** Each Member is passive by default, and can only be promoted to active, if he is invoker of any action. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float BonusPerDealPassiveParticipant = 0.01f;
	/** Bigger deals (point count) are slightly rewarded, but not too much. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float BonusPerDealPassivePoint = 0.01f;
	/** Both points and participants cap the bonus. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float PassiveCap = 0.05f;
	/** Sabotage is capped at certain value, if over 1 it can lead to negative gains from deals. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resolution")
	float SabotageCap = 1.00f;
};