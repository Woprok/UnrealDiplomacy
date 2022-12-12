// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Skirmish/UDSkirmishGameMode.h"
#include "UDDevelopmentGameMode.generated.h"

/**
 * Development version of the Skirmish Game Mode.
 * Used for testing, without breaking existing functionality.
 */
UCLASS()
class UNREALDIPLOMACY_API AUDDevelopmentGameMode : public AUDSkirmishGameMode
{
	GENERATED_BODY()
public:
	/**
	 * Default constructor used by engine.
	 */
	AUDDevelopmentGameMode();
	/**
	 * Called when the game starts or when spawned.
	 */
	virtual void BeginPlay() override;

	void PlayTest();
	virtual void Tick(float DeltaTime) override;
};