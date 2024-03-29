// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDCommandData.generated.h"

/**
 * Defines all available and supported commands for server.
 */
UENUM(BlueprintType)
enum class EUDCommandType : uint8
{
	/**
	 * Default command that will be ignored by server.
	 */
	None = 0,
	/**
	 * Server will attempt to start the game.
	 */
	StartGame = 1,
	/**
	 * Server will acknowledge that host left the game and it should do the same.
	 */
	HostClosedGame = 2,
	/**
	 * Graceful notification to server that player is about to leave the game.
	 * Server does not need to do anything special.
	 */
	PlayerLeftGame = 3,
};

/**
 * Defines all available commands for client to use.
 */
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDCommandData
{
	GENERATED_BODY()
public:
	/**
	 * Empty constructor used by UE.
	 */
	FUDCommandData();
	/**
	 * Constructs specified command.
	 */
	FUDCommandData(EUDCommandType command);
	/**
	 * Command that is server asked to execute.
	 */
	UPROPERTY(BlueprintReadOnly)
	EUDCommandType Command = EUDCommandType::None;
	/**
	 * Returns debug friendly string from this command data.
	 */
	FString ToString() const;
};