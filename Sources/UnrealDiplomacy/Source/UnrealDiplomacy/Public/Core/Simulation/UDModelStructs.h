// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDModelStructs.generated.h"

/**
 * Describes state of the session and game.
 */
UENUM(BlueprintType)
enum class EUDGameStateInfo : uint8
{
	/**
	 * Value was not changed yet.
	 */
	Undefined,
	/**
	 * Game is outside of playable state and was not yet started. 
	 */
	Lobby,
	/**
	 * Game transitioned to playable state. 
	 */
	Match,
	/**
	 * Game transitioned to non-playable state. 
	 */
	Scoreboard,
};

USTRUCT(BlueprintType)
struct FUDFactionMinimalInfo
{
	GENERATED_BODY()
public:
	FUDFactionMinimalInfo();
	FUDFactionMinimalInfo(int32 id, FString name);
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name;
};

USTRUCT(BlueprintType)
struct FUDStratagemPickableInfo
{
	GENERATED_BODY()
public:
	FUDStratagemPickableInfo();
	FUDStratagemPickableInfo(int32 id, FString name, int32 cost, bool isSelected);
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name;
	UPROPERTY(BlueprintReadOnly)
	int32 Cost;
	UPROPERTY(BlueprintReadOnly)
	bool IsSelected;
}; 

USTRUCT(BlueprintType)
struct FUDGameOverInfo
{
	GENERATED_BODY()
public:
	FUDGameOverInfo();
	UPROPERTY(BlueprintReadOnly)
	bool IsWinner = false;
	UPROPERTY(BlueprintReadOnly)
	int32 WinnerFactionId = 0;
	UPROPERTY(BlueprintReadOnly)
	FString WinnerFactionName = TEXT("");
};