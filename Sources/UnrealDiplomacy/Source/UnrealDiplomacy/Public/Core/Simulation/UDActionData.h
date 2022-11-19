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

	// TODO consider array of Imodifier that have variable implementations of fields for future data access
};
