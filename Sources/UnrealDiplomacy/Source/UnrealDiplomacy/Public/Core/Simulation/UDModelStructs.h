// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDModelStructs.generated.h"

// Forward Declarations

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
	bool IsSupporter = false;
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
	int32 MaxTurn = 0;
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
enum class EUDImperialThroneState : uint8
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
	EUDImperialThroneState State = EUDImperialThroneState::Undefined;
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
	FUDResourceInfo(int32 resourceId, FString name, int32 amount);
	UPROPERTY(BlueprintReadOnly)
	int32 Id = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 Amount = 0;
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
	/** List with deals to select from. */
	Deal,
	/** List with actions to select from. */
	DealAction,
	/** List with factions to select from. */
	FactionInvoker,
	/** List with factions to select from. */
	FactionTarget,
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
	UPROPERTY(BlueprintReadOnly)
	bool HasCurrentValue = false;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentValue = -1;
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
	UPROPERTY(BlueprintReadOnly)
	bool HasCurrentValue = false;
	UPROPERTY(BlueprintReadOnly)
	FString CurrentValue = TEXT("");
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
	UPROPERTY(BlueprintReadOnly)
	bool HasCurrentValue = false;
	UPROPERTY(BlueprintReadOnly)
	FIntPoint CurrentValue = FIntPoint(-1, -1);
};

USTRUCT(BlueprintType)
struct FUDDealParameter
{
	GENERATED_BODY()
public:
	FUDDealParameter();
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString ToolTip = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	TArray<FUDDealMinimalInfo> Options = { };
	UPROPERTY(BlueprintReadOnly)
	bool HasCurrentValue = false;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentValue = -1;
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
	UPROPERTY(BlueprintReadOnly)
	bool HasCurrentValue = false;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentValue = -1;
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
	UPROPERTY(BlueprintReadOnly)
	bool HasCurrentValue = false;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentValue = -1;
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
	UPROPERTY(BlueprintReadOnly)
	bool HasCurrentValue = false;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentValue = -1;
};

/** Question is whatever it's chicken or an egg. */
typedef TVariant<FUDDealParameter, FUDFactionParameter, FUDTileParameter, FUDActionParameter,
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
struct FUDMessageContentInfo
{
	GENERATED_BODY()
public:
	FUDMessageContentInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
	UPROPERTY(BlueprintReadOnly)
	FString Name = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString Content = TEXT("");
};

USTRUCT(BlueprintType)
struct FUDMessageInfo
{
	GENERATED_BODY()
public:
	FUDMessageInfo();
	UPROPERTY(BlueprintReadOnly)
	FString Type = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 DecisionId = 0;
	UPROPERTY(BlueprintReadOnly)
	FUDMessageContentInfo Content;
	UPROPERTY(BlueprintReadOnly)
	FUDMessageContentInfo AdditionalContent;
	UPROPERTY(BlueprintReadOnly)
	bool HasChoices = true;
	UPROPERTY(BlueprintReadOnly)
	bool HasReject = false;
public:
	/** Equality over UniqueId field. */
	inline bool operator!=(const FUDMessageInfo& rhs) const
	{
		return !(*this == rhs);
	}
	/** Equality over UniqueId field. */
	inline bool operator==(const FUDMessageInfo& rhs) const
	{
		return DecisionId == rhs.DecisionId;
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
struct FUDDealListInfo
{
	GENERATED_BODY()
public:
	FUDDealListInfo();
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

USTRUCT(BlueprintType)
struct FUDDealFactionInfo
{
	GENERATED_BODY()
public:
	FUDDealFactionInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 DealId;
	UPROPERTY(BlueprintReadOnly)
	int32 FactionId;
	UPROPERTY(BlueprintReadOnly)
	FString FactionName;
	UPROPERTY(BlueprintReadOnly)
	bool IsInviteble = false;
	UPROPERTY(BlueprintReadOnly)
	bool IsReady;
	UPROPERTY(BlueprintReadOnly)
	bool IsYesVote;
	UPROPERTY(BlueprintReadOnly)
	bool CanBeKicked;
};

USTRUCT(BlueprintType)
struct FUDDealInfo
{
	GENERATED_BODY()
public:
	FUDDealInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	bool IsModerator = false;
	UPROPERTY(BlueprintReadOnly)
	FString State;
	UPROPERTY(BlueprintReadOnly)
	int32 ReadyCount = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 VoteCount = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 ParticipantCount = 0;
	UPROPERTY(BlueprintReadOnly)
	bool LocalReady = false;
	UPROPERTY(BlueprintReadOnly)
	bool LocalVote = false;
};

USTRUCT(BlueprintType)
struct FUDDealPointMinimalInfo
{
	GENERATED_BODY()
public:
	FUDDealPointMinimalInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PointId = 0;
};

USTRUCT(BlueprintType)
struct FUDPointInteractionInfo
{
	GENERATED_BODY()
public:
	FUDPointInteractionInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = -1;
	UPROPERTY(BlueprintReadOnly)
	int32 PointId = -1;
	/** Title given to this point. */
	UPROPERTY(BlueprintReadOnly)
	FString PointTitle = TEXT("");
	/** Formatted content that uses parameters. */
	UPROPERTY(BlueprintReadOnly)
	FString PointContent = TEXT("");
	/** Point Action determines parameters. */
	UPROPERTY(BlueprintReadOnly)
	int32 PointActionTypeId = 0;
	/** Parameters are extended with deal action and invoker for full editation. */
	UPROPERTY(BlueprintReadOnly)
	FUDParameterListInfo Parameters;
	/** Defines if the point is ignored or not from all future evaluations. */
	UPROPERTY(BlueprintReadOnly)
	bool IsIgnored;
};

USTRUCT(BlueprintType)
struct FUDActionInteractionInfo
{
	GENERATED_BODY()
public:
	FUDActionInteractionInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = -1;
	UPROPERTY(BlueprintReadOnly)
	int32 PointIndex = -1;
	UPROPERTY(BlueprintReadOnly)
	FString ActionTitle = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	FString ActionContent = TEXT("");
	UPROPERTY(BlueprintReadOnly)
	int32 PointActionTypeId = 0;
	UPROPERTY(BlueprintReadOnly)
	FUDParameterListInfo Parameters;
	UPROPERTY(BlueprintReadOnly)
	bool IsInteractable;
	UPROPERTY(BlueprintReadOnly)
	bool IsSabotageable;
};

USTRUCT(BlueprintType)
struct FUDDealActionMinimalInfo
{
	GENERATED_BODY()
public:
	FUDDealActionMinimalInfo();
	UPROPERTY(BlueprintReadOnly)
	int32 DealId;
	UPROPERTY(BlueprintReadOnly)
	int32 ActionIndex;
};

/** Single selectable policy item. */
USTRUCT(BlueprintType)
struct FUDPolicySelectItemInfo
{
	GENERATED_BODY()
public:
	FUDPolicySelectItemInfo();
	/** Refers to action that is used for select. */
	UPROPERTY(BlueprintReadOnly)
	int32 SelectPolicyActionId;
	/** Id of the policy. Most likely refers to an ActionId. */
	UPROPERTY(BlueprintReadOnly)
	int32 PolicyId;
	UPROPERTY(BlueprintReadOnly)
	FString Name;
	UPROPERTY(BlueprintReadOnly)
	bool IsSelected;
};
#pragma endregion