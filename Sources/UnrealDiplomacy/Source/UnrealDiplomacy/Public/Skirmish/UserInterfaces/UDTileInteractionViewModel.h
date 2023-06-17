// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDTileInteractionViewModel.generated.h"

// Forward Declarations

struct FUDTileInteractionInfo;
class UUDParameterEditorViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDParameterEditorChanged, const TObjectPtr<UUDParameterEditorViewModel>& editorViewModel);

/**
 * Single interactable action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileInteractionViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the interaction option.
	 */
	void SetContent(FIntPoint selectedTile, FUDTileInteractionInfo content);
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
	void SetInteractText(FText newInteractText);
	FText GetInteractText() const;
private:
	// Fields
	FIntPoint SelectedTile;
	FUDTileInteractionInfo Content;
	// Current Instance in use...
	FName ParameterEditorInstanceName = TEXT("ParameterEditorInstance");
	TSubclassOf<UUDViewModel> ParameterEditorType;
	TObjectPtr<UUDParameterEditorViewModel> ParameterEditorInstance;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};