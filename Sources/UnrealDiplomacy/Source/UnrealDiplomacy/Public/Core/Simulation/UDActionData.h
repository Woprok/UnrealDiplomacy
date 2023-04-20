// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDActionData.generated.h"

#pragma region DataWrappers
/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDTargetData
{
	GENERATED_BODY()
public:
	FUDTargetData() {}
	FUDTargetData(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDValueData
{
	GENERATED_BODY()
public:
	FUDValueData() {}
	FUDValueData(TArray<int32> valueParameters)
	{
		Value = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDDealData
{
	GENERATED_BODY()
public:
	FUDDealData() {}
	FUDDealData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDDealValueData
{
	GENERATED_BODY()
public:
	FUDDealValueData() {}
	FUDDealValueData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		Value = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDDealPointData
{
	GENERATED_BODY()
public:
	FUDDealPointData() {}
	FUDDealPointData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		Point = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Point = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDDealPointValueData
{
	GENERATED_BODY()
public:
	FUDDealPointValueData() {}
	FUDDealPointValueData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		Point = valueParameters[1];
		Value = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Point = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDDealPointTileData
{
	GENERATED_BODY()
public:
	FUDDealPointTileData() {}
	FUDDealPointTileData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		Point = valueParameters[1];
		X = valueParameters[2];
		Y = valueParameters[3];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Point = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
};
/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDDealPointTileValueData
{
	GENERATED_BODY()
public:
	FUDDealPointTileValueData() {}
	FUDDealPointTileValueData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		Point = valueParameters[1];
		X = valueParameters[2];
		Y = valueParameters[3];
		Value = valueParameters[4];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Point = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
};
/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDDealTypeChangeData
{
	GENERATED_BODY()
public:
	FUDDealTypeChangeData() {}
	FUDDealTypeChangeData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		NewType = valueParameters[1];
		OldType = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 NewType = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 OldType = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDDealTargetData
{
	GENERATED_BODY()
public:
	FUDDealTargetData() {}
	FUDDealTargetData(TArray<int32> valueParameters)
	{
		DealId = valueParameters[0];
		TargetId = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 DealId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDTargetValueData
{
	GENERATED_BODY()
public:
	FUDTargetValueData() {}
	FUDTargetValueData(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		Value = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDTargetTileData
{
	GENERATED_BODY()
public:
	FUDTargetTileData() {}
	FUDTargetTileData(TArray<int32> valueParameters)
	{
		TargetId = valueParameters[0];
		X = valueParameters[1];
		Y = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 TargetId = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDTileData
{
	GENERATED_BODY()
public:
	FUDTileData() {}
	FUDTileData(TArray<int32> valueParameters)
	{
		X = valueParameters[0];
		Y = valueParameters[1];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDTileValueData
{
	GENERATED_BODY()
public:
	FUDTileValueData() {}
	FUDTileValueData(TArray<int32> valueParameters)
	{
		X = valueParameters[0];
		Y = valueParameters[1];
		Value = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 X = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Y = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Value = 0;
};

/**
 * Transforms action parameters to properly named fields.
 * @param FUDActionData.ValueParameters
 */
USTRUCT(BlueprintType)
struct FUDMapSettingsData
{
	GENERATED_BODY()
public:
	FUDMapSettingsData() {}
	FUDMapSettingsData(TArray<int32> valueParameters)
	{
		Seed = valueParameters[0];
		SizeX = valueParameters[1];
		SizeY = valueParameters[2];
	}
	UPROPERTY(BlueprintReadOnly)
	int32 Seed = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 SizeX = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 SizeY = 0;
};

#pragma endregion

/**
 * Actions are passed between Client and Server as pure data of an absolute change.
 * Struct is used to comply with default serialization and avoid further complexity with pointers.
 * Most weight is carried by ActionTypeId and InvokerPlayerId.
 * Remaning data are filled whenever they are needed. UniqueId is set by Server. Client set value is ignored!
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
	 * Copy constructor used by UE.
	 * Please do not copy actions, it will cause errors!
	 */
	FUDActionData(const FUDActionData& existingAction);
	/**
	 * Default constructor used for actions that require no additional parameters.
	 * Example: Simple actions like end turn.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerPlayerId);
	/**
	 * Constructor used for actions that require additional value parameters.
	 * Example: Most actions such as these that requires target, value etc.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerPlayerId, TArray<int32> valuePrameters);
	/**
	 * Constructor used for actions that require additional text parameter.
	 * Example: Changing faction name.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerPlayerId, FString textPrameters);
	/**
	 * Constructor used for actions that require additional value parameters and text parameter.
	 * Example: Chat messages inside of an deal.
	 */
	FUDActionData(int32 actionTypeId, int32 invokerPlayerId, TArray<int32> valuePrameters, FString textPrameters);

	/**
	 * Creates child from its parent. Usefull for action that are responses.
	 */
	static FUDActionData AsSuccessorOf(const FUDActionData& parentAction, int32 ActionTypeId);
	/**
	 * Converts action to child action.
	 * Note: existance of this is purely toxic for code health.
	 */
	//static FUDActionData AsChildOf(const FUDActionData& parentAction, int32 parentUniqueId);
	/**
	 * Creates parent from its child. Usefull for action that is reverting it's effect.
	 */
	static FUDActionData AsPredecessorOf(const FUDActionData& childAction, int32 ActionTypeId);
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
	inline bool IsValueEqual(const FUDActionData & rhs)
	{
		return 
			ActionTypeId == rhs.ActionTypeId && 
			UniqueId == rhs.UniqueId &&
			SourceUniqueId == rhs.SourceUniqueId &&
			InvokerPlayerId == rhs.InvokerPlayerId &&
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
	int32 UniqueId = 0;
	/**
	 * By default this is same as UniqueId. 
	 * For composite action this will contain UniqueId of the original action.
	 * All actions valid for execution will have this assigned to parent or to self.
	 * This is used along with UniqueId in certain cases to provide identification for objects add
	 * to the WorldState.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 SourceUniqueId = 0;
	/**
	 * Player/Ai/Server that created this action and asked for it to be executed.
	 * This is required to be filled by creator of the action or filled for him.
	 */
	UPROPERTY(BlueprintReadOnly)
	int32 InvokerPlayerId = 0;
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