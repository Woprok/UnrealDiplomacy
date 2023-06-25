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
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDFactionMinimalInfo& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDFactionMinimalInfo& rhs) const
	{
		return Id == rhs.Id;
	}
};


USTRUCT(BlueprintType)
struct FUDActionMinimalInfo
{
	GENERATED_BODY()
public:
	FUDActionMinimalInfo();
	FUDActionMinimalInfo(int32 id, FString name);
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name;
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDActionMinimalInfo& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDActionMinimalInfo& rhs) const
	{
		return Id == rhs.Id;
	}
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
USTRUCT(BlueprintType)

struct FUDResourceMinimalInfo
{
	GENERATED_BODY()
public:
	FUDResourceMinimalInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT(""); 
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDResourceMinimalInfo& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDResourceMinimalInfo& rhs) const
	{
		return Id == rhs.Id;
	}
};

#pragma endregion

USTRUCT(BlueprintType)
struct FUDTileMinimalInfo
{
	GENERATED_BODY()
public:
	FUDTileMinimalInfo();
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Position = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDTileMinimalInfo& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDTileMinimalInfo& rhs) const
	{
		return Position == rhs.Position;
	}
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
	UPROPERTY(BlueprintReadOnly)
	int32 ResourceAmount = 0;
	UPROPERTY(BlueprintReadOnly)
	FString ResourceTypeName = TEXT("");
};

#pragma region Parameters
/**
 * Describes state of the throne in relation to faction.
 */
UENUM(BlueprintType)
enum class EUDParameterType : uint8
{
	/** List with factions to select from. */
	Faction,
	/** List with tiles to select from. */
	Tile,
	/** List with actions to select from. */
	Action,
	/** List with resources to select from. */
	Resource,
	/** Editable value with min, max constrain. */
	Value,
	/** Editable text with length contrain. */
	Text,
};

USTRUCT(BlueprintType)
struct FUDValueParameter
{
	GENERATED_BODY()
public:
	FUDValueParameter();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString ToolTip = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 MinValue = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxValue = 0;
};

USTRUCT(BlueprintType)
struct FUDTextParameter
{
	GENERATED_BODY()
public:
	FUDTextParameter();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString ToolTip = TEXT("");
};

USTRUCT(BlueprintType)
struct FUDTileParameter
{
	GENERATED_BODY()
public:
	FUDTileParameter();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString ToolTip = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDTileMinimalInfo> Options = { };
};

USTRUCT(BlueprintType)
struct FUDFactionParameter
{
	GENERATED_BODY()
public:
	FUDFactionParameter();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString ToolTip = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDFactionMinimalInfo> Options = { };
};

USTRUCT(BlueprintType)
struct FUDActionParameter
{
	GENERATED_BODY()
public:
	FUDActionParameter();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString ToolTip = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDActionMinimalInfo> Options = { };
};

USTRUCT(BlueprintType)
struct FUDResourceParameter
{
	GENERATED_BODY()
public:
	FUDResourceParameter();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString ToolTip = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDResourceMinimalInfo> Options = { };
};

/** Question is whatever it's chicken or an egg. */
typedef TVariant<FUDFactionParameter, FUDTileParameter, FUDActionParameter,
	FUDResourceParameter, FUDValueParameter, FUDTextParameter> ParameterData;

USTRUCT(BlueprintType)
struct FUDParameterListInfo
{
	GENERATED_BODY()
public:
	FUDParameterListInfo();
	/** Both share same order. */
	UPROPERTY(BlueprintReadOnly)
	TArray<EUDParameterType> OrderedType;
	/** Answer is it's not a property valid type. */
	TArray<ParameterData> OrderedData;
};

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
	UPROPERTY(BlueprintReadOnly)
	FUDParameterListInfo Parameters;
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
	UPROPERTY(BlueprintReadOnly)
	FUDParameterListInfo Parameters;
};
#pragma endregion

USTRUCT(BlueprintType)
struct FUDMessageInfo
{
	GENERATED_BODY()
public:
	FUDMessageInfo();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString Content = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 RequestId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 AcceptId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 RejectId = 0;
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDMessageInfo& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDMessageInfo& rhs) const
	{
		return RequestId == rhs.RequestId;
	}
};

USTRUCT(BlueprintType)
struct FUDMessageInteractionInfo
{
	GENERATED_BODY()
public:
	FUDMessageInteractionInfo();
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDMessageInfo> Messages = { };
};

USTRUCT(BlueprintType)
struct FUDModifierInfo
{
	GENERATED_BODY()
public:
	FUDModifierInfo();
	UPROPERTY(BlueprintReadOnly)
	FString Name;
	UPROPERTY(BlueprintReadOnly)
	FString Description;
	UPROPERTY(BlueprintReadOnly)
	int32 Id;
};

#pragma region Deals

USTRUCT(BlueprintType)
struct FUDDealMinimalInfo
{
	GENERATED_BODY()
public:
	FUDDealMinimalInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDDealMinimalInfo& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDDealMinimalInfo& rhs) const
	{
		return DealId == rhs.DealId;
	}
};

USTRUCT(BlueprintType)
struct FUDDealInteractionInfo
{
	GENERATED_BODY()
public:
	FUDDealInteractionInfo();
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDDealMinimalInfo> Active = { };
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDDealMinimalInfo> History = { };
};

USTRUCT(BlueprintType)
struct FUDChatMessageInfo
{
	GENERATED_BODY()
public:
	FUDChatMessageInfo();
	UPROPERTY(BlueprintReadOnly)
	FString FactionName;
	UPROPERTY(BlueprintReadOnly)
	FString Message;
};

#pragma endregion