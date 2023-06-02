// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.generated.h"

/**
 * Actions represent changes to the world state.
 * Each action is carrying all necessary data for the action to be executed by it's executor.
 * 
 * ActionTypeId associated with executor.
 * InvokerFactionId associated wtih faction.
 * UniqueId is always set by server. Represents order of the action on the timeline.
 * 
 * Default == is defined as UniqueId compare.
 * For value compare use IsValueEqual.
 * 
 * This is struct and should serialize without any addtional serialization required.
 * Pointers should never be used in this struct as this needs to comply with standard serialization.
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
	 * Copy constructor used by UE.
	 * Please do not copy actions, it will cause errors!
	 */
	FUDActionData(const FUDActionData& existingAction);
	/**
	 * Default constructor used for actions that require no additional parameters.
	 * Example: Simple actions like end turn.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerFactionId);
	/**
	 * Constructor used for actions that require additional value parameters.
	 * Example: Most actions such as these that requires target, value etc.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerFactionId, TArray<int32> valuePrameters);
	/**
	 * Constructor used for actions that require additional text parameter.
	 * Example: Changing faction name.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerFactionId, FString textPrameters);
	/**
	 * Constructor used for actions that require additional value parameters and text parameter.
	 * Example: Chat messages inside of an deal.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerFactionId, TArray<int32> valuePrameters, FString textPrameters);
	/**
	 * Creates child from its parent. Usefull for action that are responses.
	 */
	static FUDActionData AsSuccessorOf(const FUDActionData& parentAction, int32 ActionTypeId);
	/**
	 * Creates parent from its child. Usefull for action that is reverting it's effect.
	 */
	static FUDActionData AsPredecessorOf(const FUDActionData& childAction, int32 ActionTypeId);
	/**
	 * Returns debug friendly string from this action data.
	 */
	FString ToString() const;
private:	
	 /**
	  * Copy constructor for static construction of child and parent.
	  */
	 static FUDActionData CreateDataCopy(const FUDActionData& existingAction);
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
	inline bool IsValueEqual(const FUDActionData & rhs) const
	{
		return 
			ActionTypeId == rhs.ActionTypeId && 
			UniqueId == rhs.UniqueId &&
			SourceUniqueId == rhs.SourceUniqueId &&
			InvokerFactionId == rhs.InvokerFactionId &&
			ValueParameters == rhs.ValueParameters &&
			TextParameter == rhs.TextParameter;
	}
public:
	/**
	 * Id used by action interface to determine if this action can be resolved by it.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 ActionTypeId = 0;
	/**
	 * Unique identifier, that is responsible for acting as a link between different actions.
	 * By default is assigned by WorldSimulation or copy constructor.
	 * E.g. Gift and it's eventual confirm/reject action will share this.
	 * Actually they can't share this like ever...
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 UniqueId = -1;
	/**
	 * By default this is same as UniqueId. 
	 * For composite action this will contain UniqueId of the original action.
	 * All actions valid for execution will have this assigned to parent or to self.
	 * This is used along with UniqueId in certain cases to provide identification for objects add
	 * to the WorldState.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 SourceUniqueId = -1;
	/**
	 * Player/Ai/Server that created this action and asked for it to be executed.
	 * This is required to be filled by creator of the action or filled for him.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 InvokerFactionId = -1;
	/**
	 * Optional parameters, e.g. target player id, tile, amount...
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> ValueParameters;
	/**
	 * For all actions that need to backup previous state value for revert.
	 * This generally happens only if they are making numeric value change.
	 * Server is only one authorized to fill this.
	 * This should hold smallest subset of values to enable lossless revert.
	 */
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> BackupValueParameters;
	/**
	 * Optional string, e.g. player country name, chat message...
	 */
	UPROPERTY(BlueprintReadOnly)
	FString TextParameter;
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