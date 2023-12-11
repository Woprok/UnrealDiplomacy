// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDStratagemUseManager.generated.h"

USTRUCT(BlueprintType)
struct FUDStratagemResourceCost
{
	GENERATED_BODY()
public:
	FUDStratagemResourceCost() {};
	FUDStratagemResourceCost(int32 resourceId, int32 resourceCost) : ResourceId(resourceId), ResourceCost(resourceCost) {};
	UPROPERTY(BlueprintReadWrite)
	int32 ResourceId;
	UPROPERTY(BlueprintReadWrite)
	int32 ResourceCost;
};

USTRUCT(BlueprintType)
struct FUDStratagemCost
{
	GENERATED_BODY()
public:
	FUDStratagemCost() {};
	FUDStratagemCost(TArray<FUDStratagemResourceCost> cost) : Cost(cost) {};
	UPROPERTY(BlueprintReadWrite)
	TArray<FUDStratagemResourceCost> Cost;
};

/**
 * Stratagem use manager is responsible for handling used modifiers and stratagem cost.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDStratagemUseManager : public UObject
{
public:
	GENERATED_BODY()
public:
	void DoMagic() { };
};