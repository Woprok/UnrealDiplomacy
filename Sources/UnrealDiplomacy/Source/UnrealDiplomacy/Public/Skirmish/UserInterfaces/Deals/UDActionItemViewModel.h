// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDActionItemViewModel.generated.h"

// Forward Declarations

struct FUDDealActionInfo;
class UUDParameterEditorViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDParameterEditorChanged, const TObjectPtr<UUDParameterEditorViewModel>& editorViewModel);

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDActionItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(FUDDealActionInfo content);
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
	FText AcceptText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ChangeText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText DenyText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SabotageText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText EditorText;
	// Events
	FUDParameterEditorChanged ParameterEditorChangedEvent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/** Updates all parameter lists. */
	void UpdateEditor();
	/** Defines all parameter instances. */
	void DefineInstances();
private:
	// MVVM Setters & Getters
	void SetActionText(FText newActionText);
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
private:
	// Fields
	FUDDealActionInfo Content;
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