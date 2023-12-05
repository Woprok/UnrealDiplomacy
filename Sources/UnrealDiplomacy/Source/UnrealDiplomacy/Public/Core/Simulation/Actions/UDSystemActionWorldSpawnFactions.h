// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/Simulation/Actions/UDSystemAction.h"
#include "UDSystemActionWorldSpawnFactions.generated.h"

/**
 * Spawns each playable faction (AI or PC) on the map.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDSystemActionWorldSpawnFactions : public UUDSystemAction
{
	GENERATED_BODY()
public:
	virtual void Execute(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual void Revert(const FUDActionData& action, TObjectPtr<UUDWorldState> world) override;
	virtual int32 GetId() const override { return ActionTypeId; };
	virtual void SetWorldGenerator(TWeakObjectPtr<UUDWorldGenerator> worldGenerator) override;
	virtual void SetWorldFactionGenerator(TWeakObjectPtr<UUDWorldFactionGenerator> worldFactionGenerator) override;
public:
	static const int32 ActionTypeId = 24;
protected:
	UPROPERTY()
	TWeakObjectPtr<UUDWorldGenerator> WorldGenerator = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UUDWorldFactionGenerator> WorldFactionGenerator = nullptr;
};