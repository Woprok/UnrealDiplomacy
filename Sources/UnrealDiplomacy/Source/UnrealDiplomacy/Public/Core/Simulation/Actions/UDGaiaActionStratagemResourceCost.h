// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDGaiaAction.h"
#include "UDGaiaActionTileUpkeep.generated.h"

USTRUCT(BlueprintType)
struct FUDResourceUpkeep
{
	GENERATED_BODY()
public:
	FUDResourceUpkeep() {};
	FUDResourceUpkeep(int32 resourceId, int32 resourceUpkeep) : ResourceId(resourceId), ResourceUpkeep(resourceUpkeep) {};
	UPROPERTY(BlueprintReadWrite)
	int32 ResourceId;
	UPROPERTY(BlueprintReadWrite)
	int32 ResourceUpkeep;
};

USTRUCT(BlueprintType)
struct FUDUpkeep
{
	GENERATED_BODY()
public:
	FUDUpkeep() {};
	FUDUpkeep(TArray<FUDResourceUpkeep> upkeeps) : Upkeeps(upkeeps) {};
	UPROPERTY(BlueprintReadWrite)
	TArray<FUDResourceUpkeep> Upkeeps;
};

/**
 * Penalzies faction for each tile they own.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDGaiaActionTileUpkeep : public UUDGaiaAction
{
	GENERATED_BODY()
public:
	UUDGaiaActionTileUpkeep();
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
	/** TODO change this... This is binding tile type to resource upkeep4...  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Config")
	TMap<int32, FUDUpkeep> TileUpkeeps = { };
};