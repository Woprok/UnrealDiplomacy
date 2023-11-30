// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDWorldArbiter.generated.h"

// Forward Declarations
struct FUDActionData;
class UUDWorldState;
class UUDModifierManager;
class UUDResourceManager;

USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDArbiterRuleset
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	int32 MaxTurnCount = 12;
};

/**
 * The only absolute authority over the execution flow.
 * Is responsible for determining if the WorldState reached a point of victory.
 * There is currently only one way to win the game by being the holder of the throne.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDWorldArbiter : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * Requires Gaia world state to provide accurate information.
	 */
	bool OnActionExecutionFinished(int32 actionType, const TObjectPtr<UUDWorldState>& gaiaWorldState);
	TArray<FUDActionData> EndGame();
	void SetModifierManager(TObjectPtr<UUDModifierManager> modifierManager);
	void SetResourceManager(TObjectPtr<UUDResourceManager> resourceManager);
protected:
	void EvaluateTurnGameOverState(const TObjectPtr<UUDWorldState>& gaiaWorldState);
	FUDActionData DetermineNewRuler();
	FUDActionData CreateEndGame();
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;
private:
	bool GameReachedEnd = false;
	bool CrownIsEmpty = false;
	int32 CrownableRuler = 0;
	FUDArbiterRuleset CurrentRuleSet = FUDArbiterRuleset();
	int32 GetTotalSupport(const TObjectPtr<UUDWorldState>& state, int32 factionId);
};