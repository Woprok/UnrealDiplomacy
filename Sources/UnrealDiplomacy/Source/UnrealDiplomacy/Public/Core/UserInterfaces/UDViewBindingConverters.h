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
	/** Returns true if the provided values are same. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Value To Bool", CompactNodeTitle = "ValueToBool", BlueprintAutocast), Category = "Convertors")
	static bool ValueToBool(int32 InValue, int32 PrimaryValue);
	/** Returns true if the provided values are not same. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Value To Not Bool", CompactNodeTitle = "ValueToNotBool", BlueprintAutocast), Category = "Convertors")
	static bool ValueToNotBool(int32 InValue, int32 PrimaryValue);


	/** If bPickA is true, A is returned, otherwise B is */
	//UFUNCTION(BlueprintPure, Category = "Math|Color")
	//static FLinearColor SelectColor(FLinearColor A, FLinearColor B, bool bPickA);
};