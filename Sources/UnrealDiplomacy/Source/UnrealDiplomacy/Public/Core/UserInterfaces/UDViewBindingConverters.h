// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "UDViewBindingConverters.generated.h"

// Forward Declarations

enum class ESlateVisibility : uint8;

/**
 * Collection of standard data type convertors.
 */
UCLASS(meta = (BlueprintThreadSafe, ScriptName = "ViewBindingConvertors"))
class UNREALDIPLOMACY_API UUDViewBindingConverters : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, meta = (DisplayName = "True as Visible Visibility", CompactNodeTitle = "BoolToVisbility", BlueprintAutocast), Category = "Convertors")
	static ESlateVisibility BoolToVisible(bool InBool);
	UFUNCTION(BlueprintPure, meta = (DisplayName = "True as Collapsed Visibility", CompactNodeTitle = "BoolToInvisibility", BlueprintAutocast), Category = "Convertors")
	static ESlateVisibility BoolToCollapsed(bool InBool);
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Visibility Visible To True", CompactNodeTitle = "VisibilityToBool", BlueprintAutocast), Category = "Convertors")
	static bool VisibleToBool(ESlateVisibility InVisibility);
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Visibility Collapsed To True", CompactNodeTitle = "InVisibilityToBool", BlueprintAutocast), Category = "Convertors")
	static bool CollapsedToBool(ESlateVisibility InVisibility);
};