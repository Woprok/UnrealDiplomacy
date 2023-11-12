// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDFactionInteractionViewModel.generated.h"

// Forward Declarations

enum class EUDDecisionType : uint8;
struct FUDFactionInteractionInfo;
struct FUDActionData;
class UUDParameterEditorViewModel;

/**
 * Single interactable action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionInteractionViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the interaction option.
	 */
	void SetContent(int32 selectedFaction, EUDDecisionType interactionType, FUDFactionInteractionInfo content);
public:
	// Button Functions
	/**
	 * Execution of the interaction.
	 */
	UFUNCTION()
	void Interact();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText InteractText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FUDViewModelContent ParameterEditorContent;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	/** Interacts based on current InteractionType */
	void DecisionRequest(FUDActionData data);
	/** Updates all parameter lists. */
	void UpdateEditor();
	/** Defines all parameter instances. */
	void DefineInstances();
private:
	// MVVM Setters & Getters
	void SetInteractText(FText newInteractText);
	FText GetInteractText() const;
	void SetParameterEditorContent(FUDViewModelContent newParameterEditorContent);
	FUDViewModelContent GetParameterEditorContent() const;
private:
	// Fields
	int32 SelectedFaction;
	EUDDecisionType InteractionType;
	FUDFactionInteractionInfo Content;
	// Current Instance in use...
	FName ParameterEditorInstanceName = TEXT("FactionParameterEditorInstance");
	TSubclassOf<UUDViewModel> ParameterEditorType;
	TObjectPtr<UUDParameterEditorViewModel> ParameterEditorInstance;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};