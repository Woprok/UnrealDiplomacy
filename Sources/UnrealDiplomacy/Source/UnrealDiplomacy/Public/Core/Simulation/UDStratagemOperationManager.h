// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDStratagemOperationManager.generated.h"

// Forward Declarations

struct FUDActionData;
struct FUDActionPresentation;
class IUDActionInterface;
class UUDWorldState;
class UUDFactionState;
class UUDModifierManager;
class UUDResourceManager;

USTRUCT(BlueprintType)
struct FUDStratagemResourceCost
{
	GENERATED_BODY()
public:
	FUDStratagemResourceCost() {};
	FUDStratagemResourceCost(int32 resourceId, int32 resourceCost) : ResourceId(resourceId), ResourceCost(resourceCost) {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "Help field for designers, to know which resource is using this cost."))
	FName Note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResourceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResourceCost;
};

USTRUCT(BlueprintType)
struct FUDStratagemCost
{
	GENERATED_BODY()
public:
	FUDStratagemCost() {};
	FUDStratagemCost(TArray<FUDStratagemResourceCost> costs) : Costs(costs) {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "Help field for designers, to know which action is using these costs."))
	FName Note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FUDStratagemResourceCost> Costs = { };
};

/**
 * Stratagem use manager is responsible for handling used modifiers and stratagem cost.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDStratagemOperationManager : public UObject
{
public:
	GENERATED_BODY()
public:
	/** Determines if the action needs to be handlded by this manager. */
	bool IsStratagem(const FUDActionPresentation& details) const;
	/** Determines if the action needs to be handlded by this manager. */
	bool HasCost(const TObjectPtr<UUDWorldState>& world, const FUDActionPresentation& detail, const FUDActionData& action) const;
	/** Determines if this action can be executed or will be halted by this manager. */
	bool CanStratagemBeActivated(const TObjectPtr<UUDWorldState>& world, const FUDActionPresentation& details, const FUDActionData& action) const;
	/** Creates modifiers that prevent another execution of the action, e.g. in this phase... */
	TArray<FUDActionData> CreateStratagemConsequences(const FUDActionPresentation& details, const FUDActionData& action) const;	
	/** Creates modifiers that prevent another execution of the action, e.g. in this phase... */
	TArray<FUDActionData> CreateCostConsequences(const FUDActionPresentation& details, const FUDActionData& action) const;

	void SetModifierManager(TWeakObjectPtr<UUDModifierManager> modifierManager);
	void SetResourceManager(TWeakObjectPtr<UUDResourceManager> resourceManager);
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UUDResourceManager> ResourceManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optional Action Costs", meta = (Tooltip = "Map containing action IDs and associated costs."))
	TMap<int32, FUDStratagemCost> StratagemCosts = { };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Action Costs", meta = (Tooltip = "Map containing default costs, used as fallback."))
	FUDStratagemCost DefaultStratagemCost;

private:
	bool CanPayActivationCost(const TObjectPtr<UUDFactionState>& faction, const TArray<FUDStratagemResourceCost>& costs) const;
	FUDActionData GetActivationCostAction(int32 factionId, const TArray<FUDStratagemResourceCost>& costs) const;
	bool HasStratagemFromOtherFaction(const TObjectPtr<UUDFactionState>& faction, int32 stratagemId) const;
};