// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UDAIController.h"
#include "UDSkirmishAIController.generated.h"

/**
 * Default controller for AI controlled factions.
 * With very basic gift focused AI...
 */
UCLASS()
class UNREALDIPLOMACY_API AUDSkirmishAIController : public AUDAIController
{
	GENERATED_BODY()
public:
protected:
	// Action Execution
	virtual void ProcessPreGamePlay() override;
	virtual void ProcessOutTurnPlay() override;
	virtual void ProcessInTurnPlay() override;
private:
	void ResolveRequests();
	/** Returns -1 if empty. */
	int32 GetFirst(const TSet<int32>& iteratedSet);
	/** Returns -1 if not found any intersecting. Item is returned from first set. */
	int32 GetFirstIntersecting(const TSet<int32>& iteratedSet, const TSet<int32>& checkedSet);
private:
	// AI Memory fields.
	TSet<int32> FavoriteFactions = { };
	TSet<int32> EnemyFactions = { };
	TSet<int32> Supports = { };
	bool SetSettings = false;
	TArray<FString> Names = {
		TEXT("Friednly Cat"),
		TEXT("Friendly Dog"),
		TEXT("Friendly Pigeon"),
		TEXT("Friendly Raven"),

		TEXT("Friendly Elephant"),
		TEXT("Friendly Horse"),
		TEXT("Friendly Donkey"),
		TEXT("Friendly Fish"),

		TEXT("Friendly Goldfish"),
		TEXT("Friendly Rat"),
		TEXT("Friendly Mouse"),
		TEXT("Friendly Beever"),

		TEXT("Friendly Giraffe"),
		TEXT("Friendly Rhino"),
		TEXT("Friendly Snake"),
		TEXT("Friendly Cobra"),

		TEXT("Friendly Capybara"),
		TEXT("Friendly Hippopotamus"),
		TEXT("Friendly Monkey"),
		TEXT("Friendly Gorilla"),

		TEXT("Friendly Bear"),
		TEXT("Friendly Wolf"),
		TEXT("Friendly Dragon"),
		TEXT("Friendly Hippogriff"),
		
		TEXT("Friendly Fenris"),
		TEXT("Friendly Phoenix"),
		TEXT("Friendly Unicorn"),
		TEXT("Friendly Pegasus"),

		TEXT("Giftgiver Bot - 4000"),
	};
};