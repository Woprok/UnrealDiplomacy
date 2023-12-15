// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDGlobalData.generated.h"

/**
 * Collection of constants that are used as special values across the project.
 * Primary purpose of these constants is to define invalid values to avoid
 * ambiquity of whatever something is present & correct or not present.
 */
UCLASS(meta = (BlueprintThreadSafe, ScriptName = "UDGlobalData"))
class UNREALDIPLOMACY_API UUDGlobalData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/** This Id is always used by first action in action list / history. */
	static const int32 FirstUseableActionId = 0;
	/** This is first Id that can be assigned to player / AI Faction. */
	static const int32 FirstUseableFactionId = 2;
	/** This is first Id that can be assigned to player / AI State. */
	static const int32 FirstUseableStateId = 2;
	/** This is first Id that can be assigned to player / AI Controller. */
	static const int32 FirstUseableControllerId = 1;

	/** This Id is always considered invalid for all action operations. */
	static const int32 InvalidActionId = -1;
	/** This Id is always considered invalid for all faction operations. */
	static const int32 InvalidFactionId = -1;
	/** This Id is always considered invalid for all state operations. */
	static const int32 InvalidStateId = -1;
	/** This Id is always considered invalid for all controller operations. */
	static const int32 InvalidControllerId = -1;

	/** This Id is always reserved for Gaia / Server Faction. */
	static const int32 GaiaFactionId = 0;     
	/** This Id is always reserved for Gaia / Server State. */
	static const int32 GaiaStateId = 0;	      
	/** This Id is always reserved for Gaia / Server Controller. */
	static const int32 GaiaControllerId = 0;

	/** This Id is always reserved for Observer Faction. */
	static const int32 ObserverFactionId = 1;
	/** This Id is always reserved for Observer State. */
	static const int32 ObserverStateId = 1;

	/** This Id is always considered invalid for all tile operations. */
	static const int32 InvalidTilePositionX = -1;
	static const int32 InvalidTilePositionY = -1;
	/** This Index is always considered invalid for all parameter index operations. */
	static const int32 InvalidParameterArrayIndex = -1;
	/** This Index is always considered invalid for all index operations. */
	static const int32 InvalidArrayIndex = -1;
	/** This Id is always considered invalid for all resource operations. */
	static const int32 InvalidResourceId = -1;
	/** This Id is always considered invalid for all modifier operations. */
	static const int32 InvalidModifierId = -1;
	/** This Id is always considered invalid for all deal operations. */
	static const int32 InvalidDealId = -1;
	/** This Id is always considered invalid for all deal point operations. */
	static const int32 InvalidDealPointId = -1;
public:
	// Default values that are expected to exists or be used.

	/**
	 * Amount of optional parameters required for actions.
	 */
	static const int32 DefaultActionParameterCount = 0;
	/**
	 * Amount of optional parameters required for modifiers.
	 */
	static const int32 DefaultModifierParameterCount = 0;
	/**
	 * Default level name that should be same as default level.
	 */
	static const FString DefaultLevelName()
	{
		return TEXT("Menu");
	}
	/**
	 * Option required for session travel.
	 */
	static const FString ListenOption()
	{
		return TEXT("listen");
	}
	/** Shortcut for Gaia Id. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Gaia Faction Id", CompactNodeTitle = "GetGaiaFactionId", BlueprintAutocast), Category = "UDGlobalData")
	static int32 GetGaiaFactionId()
	{
		return GaiaFactionId;
	}
};

/**
 * Few optional configs, that change behaviour of systems.
 */
UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDGlobalGameConfig : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static const bool IsDirectExecutionEnabled = true;
};

// Following types are wrappers around otherwise engine types or values.

/**
 * Wrapper for all supported window modes.
 */
UENUM(BlueprintType)
enum class EUDWindowModeType : uint8
{
	/**
	 *
	 */
	Fullscreen,
	/**
	 *
	 */
	Borderless,
	/**
	 *
	 */
	Windowed,
};

/**
 * Custom type that can hold most of the application settings with non-engine references.
 */
USTRUCT(BlueprintType)
struct FUDApplicationSettings
{
	GENERATED_BODY()
public:
	FUDApplicationSettings() { }
	FUDApplicationSettings(FIntPoint resolution, EUDWindowModeType windowMode)
		: Resolution(resolution), WindowMode(windowMode) { }
	UPROPERTY(BlueprintReadOnly)
	FIntPoint Resolution;
	UPROPERTY(BlueprintReadOnly)
	EUDWindowModeType WindowMode;
	static FUDApplicationSettings GetDefault();
};

UCLASS(meta = (BlueprintThreadSafe))
class UNREALDIPLOMACY_API UUDApplicationConverters : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * Converts from engine enum to module enum.
	 */
	static EUDWindowModeType ToWindowModeType(EWindowMode::Type windowMode);
	/**
	 * Converts from module enum to engine enum.
	 */
	static EWindowMode::Type FromWindowModeType(EUDWindowModeType windowMode);
	/**
	 * Converts from engine enum to bool.
	 */
	static bool FromCheckBoxState(ECheckBoxState checkedState);
	/**
	 * Converts from bool to engine enum.
	 */
	static ECheckBoxState ToCheckBoxState(bool isChecked);
};