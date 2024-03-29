// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDActionItemViewModel.generated.h"

// Forward Declarations

struct FUDDealActionMinimalInfo;
struct FUDActionInteractionInfo;
class UUDParameterEditorViewModel;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDActionItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the strategy option. */
	void SetContent(FUDDealActionMinimalInfo content);
public:
	// Button Functions
	UFUNCTION()
	void Accept();
	UFUNCTION()
	void Change();
	UFUNCTION()
	void Deny();
	UFUNCTION()
	void Sabotage();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ActionText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ActionTitleText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText AcceptText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ChangeText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DenyText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SabotageText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText EditorText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanAcceptValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanChangeValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanDenyValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanSabotageValue;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent ParameterEditorContent;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	/** Updates all parameter lists. */
	void UpdateEditor();
	/** Defines all parameter instances. */
	void DefineInstances();
private:
	void SaveValuesChange(TArray<int32> values);
	void SaveTextChange(FString text);
private:
	// MVVM Setters & Getters
	void SetActionText(FText newActionText);
	FText GetActionTitleText() const;
	void SetActionTitleText(FText newActionTitleText);
	FText GetActionText() const;
	void SetAcceptText(FText newAcceptText);
	FText GetAcceptText() const;
	void SetChangeText(FText newChangeText);
	FText GetChangeText() const;
	void SetDenyText(FText newDenyText);
	FText GetDenyText() const;
	void SetSabotageText(FText newSabotageText);
	FText GetSabotageText() const;
	void SetEditorText(FText newEditorText);
	FText GetEditorText() const;
	void SetCanAcceptValue(bool newCanAcceptValue);
	bool GetCanAcceptValue() const;
	void SetCanChangeValue(bool newCanChangeValue);
	bool GetCanChangeValue() const;
	void SetCanDenyValue(bool newCanDenyValue);
	bool GetCanDenyValue() const;
	void SetCanSabotageValue(bool newCanSabotageValue);
	bool GetCanSabotageValue() const;
	void SetParameterEditorContent(FUDViewModelContent newParameterEditorContent);
	FUDViewModelContent GetParameterEditorContent() const;
private:
	// Fields
	FUDActionInteractionInfo Content;
	TArray<int32> BufferedValueParameters;
	FString BufferedTextParameter;
	// Current Instance in use...
	FName ParameterEditorInstanceName = TEXT("ActionParameterEditorInstance");
	TSubclassOf<UUDViewModel> ParameterEditorType;
	TObjectPtr<UUDParameterEditorViewModel> ParameterEditorInstance;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};