// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.generated.h"

/**
 * Defines data passed to a an action.
 * TODO maybe change this to class, so we can use inheritance/variants.
 * TODO consider array of Imodifier that have variable implementations of fields for future data access
 *
 * Note: default comparison == is done based purely on UniqueId, if you need complete value comparison
 * use IsValueEqual...
 */
USTRUCT(BlueprintType)
struct UNREALDIPLOMACY_API FUDActionData
{
	GENERATED_BODY()
public:
	/**
	 * Empty constructor used by UE. 
	 */
	FUDActionData();
	/**
	 * Copy constructor used by actions, that require multiple execution steps to carry over data.
	 */
	FUDActionData(const FUDActionData& existingAction, int32 ActionTypeId);
	/**
	 * Constructor for generic server/automatic use of an action.
	 * Example: StartGame (only actionTypeId is relevant for execution)
	 */
	FUDActionData(int32 actionTypeId);
	/**
	 * Constructor for generic use.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerPlayerId);
	/**
	 * Constructor for generic target use.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerPlayerId, int32 targetPlayerId);
	/**
	 * Constructor for target use with value attached.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerPlayerId, int32 targetPlayerId, int32 valueParameter);
	/**
	 * Constructor for target use with value attached and tile.
	 * TODO rethink how to pass the parameters...
	 */
	FUDActionData(int32 actionTypeId, int32 valueParameter, FIntPoint tileParameter);
	/**
	 * Constructor for source and target as well as Tile.
	 * TODO rethink how to pass the parameters...
	 */
	FUDActionData(int32 actionTypeId, int32 invokerPlayerId, int32 targetPlayerId, FIntPoint tileParameter);
	/**
	 * Unique specifier for each action type is defined by implementation.
	 * This specifier is used by IUDAction interface to determine, which one is
	 * supposed to execute this data.
	 */
	static FUDActionData Create(int32 actionTypeId, int32 invokerPLayerId, FIntPoint tileParameter)
	{
		FUDActionData data;
		data.ActionTypeId = actionTypeId;
		data.InvokerPlayerId = invokerPLayerId;
		data.TileParameter = tileParameter;
		return data;
	}
public:
	/**
	 * Equality over UniqueId field.
	 */
	inline bool operator!=(const FUDActionData & rhs)
	{
		return !(*this == rhs);
	}
	/**
	 * Equality over UniqueId field.
	 */
	inline bool operator==(const FUDActionData & rhs)
	{
		return UniqueId == rhs.UniqueId;
	}
	/**
	 * Equality over all fields.
	 */
	inline bool IsValueEqual(const FUDActionData & rhs)
	{
		return UniqueId == rhs.UniqueId &&
			ActionTypeId == rhs.ActionTypeId &&
			InvokerPlayerId == rhs.InvokerPlayerId &&
			TargetPlayerId == rhs.TargetPlayerId &&
			ValueParameter == rhs.ValueParameter;
	}
public:
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
	/**
	 * Player/Ai/Server that created this action and asked for it to be executed.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 InvokerPlayerId = 0;
	/**
	 * Player/Ai/Server that are targeted by this action.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 TargetPlayerId = 0;
	/**
	 * Simple numerical parameter.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 ValueParameter = 0;
	/**
	 * Simple tile parameter, two bundled ints.
	 */
	UPROPERTY(BlueprintReadOnly)
	FIntPoint TileParameter = FIntPoint(0);
	/**
	 * Unique identifier, that is responsible for acting as a link between different actions.
	 * By default is assigned by WorldSimulation or copy constructor.
	 * E.g. Gift and it's eventual confirm/reject action will share this.
	 * Actually they can't share this like ever...
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 UniqueId = 0;
	/**
	 * UniqueId associated with the parent Action. 
	 * Default value is 0 (not assigned, has no parent).
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 ParentUniqueId = 0;
};

/**
 * Wrapper around multiple actions, that are send as package. 
 * e.g. sending history. 
 */
USTRUCT()
struct UNREALDIPLOMACY_API FUDActionArray
{
	GENERATED_BODY()
public:
	/**
	 * Empty constructor used by UE.
	 */
	FUDActionArray()
	{

	};
	/**
	 * Custom constructor.
	 */
	FUDActionArray(TArray<FUDActionData> actions, int32 first, int32 last)
		: Actions(actions), FirstActionId(first), LastActionId(last)
	{

	}
public:
	UPROPERTY()
	TArray<FUDActionData> Actions;
	UPROPERTY()
	int32 FirstActionId = 0;
	UPROPERTY()
	int32 LastActionId = 0;
};