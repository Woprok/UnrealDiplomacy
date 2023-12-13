// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDStratagemOperationManager.generated.h"

// Forward Declarations

struct FUDActionData;
struct FUDActionPresentation;
class IUDActionInterface;
class UUDWorldState;
class UUDModifierManager;

USTRUCT(BlueprintType)
struct FUDStratagemResourceCost
{
	GENERATED_BODY()
public:
	FUDStratagemResourceCost() {};
	FUDStratagemResourceCost(int32 resourceId, int32 resourceCost) : ResourceId(resourceId), ResourceCost(resourceCost) {};
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
	FUDStratagemCost(TArray<FUDStratagemResourceCost> cost) : Cost(cost) {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Tooltip = "Help field for designers, to know which action is using this cost."))
	FName Note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FUDStratagemResourceCost> Cost = { };
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
	bool IsStratagemOrHasCost(const FUDActionPresentation& details) const;
	/** Determines if this action can be executed or will be halted by this manager. */
	bool CanStratagemBeActivated(const TObjectPtr<UUDWorldState>& world, const FUDActionPresentation& details, const FUDActionData& action) const;
	/** Creates modifiers that prevent another execution of the action, e.g. in this phase... */
	TArray<FUDActionData> CreateConsequences(const TObjectPtr<UUDWorldState>& world, const FUDActionPresentation& details, const FUDActionData& action) const;

protected:
	UPROPERTY()
	TWeakObjectPtr<UUDModifierManager> ModifierManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optional Action Costs", meta = (Tooltip = "Map containing action IDs and associated costs."))
	TMap<int32, FUDStratagemCost> StratagemCosts = { };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default Action Costs", meta = (Tooltip = "Map containing default costs, used as fallback."))
	FUDStratagemCost DefaultStratagemCost;
};