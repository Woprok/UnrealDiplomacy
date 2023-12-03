// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
#include "UDViewBindingConverters.generated.h"

// Forward Declarations

enum class ESlateVisibility : uint8;
class UImage;
struct FUDMaterialBinding;

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
	/** Returns true if the provided values are same. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Value To Bool", CompactNodeTitle = "ValueToBool", BlueprintAutocast), Category = "Convertors")
	static bool ValueToBool(int32 InValue, int32 PrimaryValue);
	/** Returns true if the provided values are not same. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Value To Not Bool", CompactNodeTitle = "ValueToNotBool", BlueprintAutocast), Category = "Convertors")
	static bool ValueToNotBool(int32 InValue, int32 PrimaryValue);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Material To Icon", CompactNodeTitle = "MaterialToIcon", BlueprintAutocast), Category = "Convertors")
	static FSlateBrush MaterialToIcon(FUDMaterialBinding icon, float x = 32.0f, float y = 32.0f);
};