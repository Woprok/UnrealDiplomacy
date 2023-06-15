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

USTRUCT(BlueprintType)
struct FUDFactionInfo
{
	GENERATED_BODY()
public:
	FUDFactionInfo();
	FUDFactionInfo(int32 id, FString name, bool isPlayerControlled);
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	/**
	 * True indicates that it's owned by player, false by AI.
	 * Observer and Server should not be returned in this structure.
	 */
	UPROPERTY(BlueprintReadOnly)
	bool IsPlayerControlled = false;
};

USTRUCT(BlueprintType)
struct FUDRegencyTurnInfo
{
	GENERATED_BODY()
public:
	FUDRegencyTurnInfo();
	UPROPERTY(BlueprintReadOnly)
	bool IsRegent = false;
	UPROPERTY(BlueprintReadOnly)
	FString RegentFactionName = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 Turn = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentFinished = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 MaximumFinished = 0;
	UPROPERTY(BlueprintReadOnly)
	bool IsFinished = true;
};

/**
 * Describes state of the throne in relation to faction.
 */
UENUM(BlueprintType)
enum class EUDThroneState : uint8
{
	/**
	 * Value was not changed yet and should be ignored.
	 */
	Undefined,
	/**
	* Throne is still vacant, or is vacant again.
	*/
	Empty,
	/**
	 * Occupied by someone else. So we do not view it positively.
	 */
	Usurper,
	/**
	 * Is currently sitting on the throne.
	 */
	Claimer,
};

USTRUCT(BlueprintType)
struct FUDThroneInfo
{
	GENERATED_BODY()
public:
	FUDThroneInfo();
	UPROPERTY(BlueprintReadOnly)
	EUDThroneState State = EUDThroneState::Undefined;
	UPROPERTY(BlueprintReadOnly)
	FString FactionName = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	bool CanInteract = false;
};

USTRUCT(BlueprintType)
struct FUDAvailableDiplomacyInfo
{
	GENERATED_BODY()
public:
	FUDAvailableDiplomacyInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 DealCount = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 MessageCount = 0;
};

#pragma region Resources
#define UD_RESOURCE_REPUTATION_ID 0
#define UD_RESOURCE_GOLD_ID 1
#define UD_RESOURCE_FOOD_ID 2
#define UD_RESOURCE_MATERIALS_ID 3
#define UD_RESOURCE_LUXURIES_ID 4
/**
 * Resource holder that is taking resources and providing all additional informations.
 * TODO this should be refactored if resources are changed to separate class that is able to
 * provide informations about itself.
 * Intended to use with static constructor, providing the approriate resource amount.
 */
USTRUCT(BlueprintType)
struct FUDResourceInfo
{
	GENERATED_BODY()
public:
	FUDResourceInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 Amount = 0;
	/** Reputation emplaced in wrapper */
	static FUDResourceInfo GetReputation(int32 amount);
	/** Gold emplaced in wrapper */
	static FUDResourceInfo GetGold(int32 amount);
	/** Food emplaced in wrapper */
	static FUDResourceInfo GetFood(int32 amount);
	/** Materials emplaced in wrapper */
	static FUDResourceInfo GetMaterials(int32 amount);
	/** Luxuries emplaced in wrapper */
	static FUDResourceInfo GetLuxuries(int32 amount);
};
#pragma endregion

USTRUCT(BlueprintType)
struct FUDFactionInteractionInfo
{
	GENERATED_BODY()
public:
	FUDFactionInteractionInfo();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
};

USTRUCT(BlueprintType)
struct FUDTileInteractionInfo
{
	GENERATED_BODY()
public:
	FUDTileInteractionInfo();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
};

USTRUCT(BlueprintType)
struct FUDTileInfo
{
	GENERATED_BODY()
public:
	FUDTileInfo();
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Position = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 FactionId = 0;
	UPROPERTY(BlueprintReadOnly)
	FString FactionName = TEXT("");
};