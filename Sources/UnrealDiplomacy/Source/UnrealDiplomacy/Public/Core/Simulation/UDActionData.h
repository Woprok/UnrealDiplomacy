// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.generated.h"

/**
 * Defines data passed to a an action.
 * TODO maybe change this to class, so we can use inheritance/variants.
 */
USTRUCT()
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
	 * Unique specifier for each action type is defined by implementation.
	 * This specifier is used by IUDAction interface to determine, which one is
	 * supposed to execute this data.
	 */
	UPROPERTY()
	int32 ActionTypeId;
	/**
	 * Player/Ai/Server that created this action and asked for it to be executed.
	 */
	UPROPERTY()
	int32 InvokerPlayerId;
	/**
	 * Player/Ai/Server that are targeted by this action.
	 */
	UPROPERTY()
	int32 TargetPlayerId;
	/**
	 * Simple numerical parameter.
	 */
	UPROPERTY()
	int32 ValueParameter;
	/**
	 * Unique identifier, that is responsible for acting as a link between different actions.
	 * By default is assigned by WorldSimulation or copy constructor.
	 * E.g. Gift and it's eventual confirm/reject action will share this.
	 */
	UPROPERTY()
	int32 UniqueId = 0;

	// TODO consider array of Imodifier that have variable implementations of fields for future data access

	inline bool operator!=(const FUDActionData& rhs) 
	{ 
		return !(*this == rhs); 
	}

	inline bool operator==(const FUDActionData& rhs)
	{ 
		return UniqueId == rhs.UniqueId 
			// &&
			// ActionTypeId == rhs.ActionTypeId &&
			// InvokerPlayerId == rhs.InvokerPlayerId &&
			// TargetPlayerId == rhs.TargetPlayerId &&
			// ValueParameter == rhs.ValueParameter
			;
	}
};
