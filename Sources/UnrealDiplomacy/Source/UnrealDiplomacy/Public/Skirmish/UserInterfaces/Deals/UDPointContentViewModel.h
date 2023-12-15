// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDPointContentViewModel.generated.h"

// Forward Declarations

struct FUDDealPointMinimalInfo;
struct FUDPointInteractionInfo;
class UUDParameterEditorViewModel;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDPointContentViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(FUDDealPointMinimalInfo content);
public:
	// Button Functions
	UFUNCTION()
	void Ignore();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PointTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText PointText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText EditorText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsValidContentValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Getter)
	FUDViewModelContent ParameterEditorContent;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText IgnoredText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsIgnoredValue;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	/** Updates all parameter lists. */
	void UpdateEditor();
	/** Defines all parameter instances. */
	void DefineInstances();
private:
	void SaveDealActionChange(int32 actionId);
	void SaveInvokerChange(int32 invokerId);
	void SaveValuesChange(TArray<int32> values);
	void SaveTextChange(FString text);
private:
	// MVVM Setters & Getters
	void SetPointTitleText(FText newPointTitleText);
	FText GetPointTitleText() const;
	void SetPointText(FText newPointText);
	FText GetPointText() const;
	void SetEditorText(FText newEditorText);
	FText GetEditorText() const;
	void SetIsValidContentValue(bool newIsValidContentValue);
	bool GetIsValidContentValue() const;
	void SetParameterEditorContent(FUDViewModelContent newParameterEditorContent);
	FUDViewModelContent GetParameterEditorContent() const;
	void SetIgnoredText(FText newIgnoredText);
	FText GetIgnoredText() const;
	void SetIsIgnoredValue(bool newIsIgnoredValue);
	bool GetIsIgnoredValue() const;
private:
	// Fields
	FUDPointInteractionInfo Content;
	// Current Instance in use...
	FName ParameterEditorInstanceName = TEXT("PointParameterEditorInstance");
	TSubclassOf<UUDViewModel> ParameterEditorType;
	TObjectPtr<UUDParameterEditorViewModel> ParameterEditorInstance;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};