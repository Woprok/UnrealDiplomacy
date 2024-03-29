// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/UserInterfaces/UDViewModelContent.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDTileInteractionViewModel.generated.h"

// Forward Declarations

struct FUDTileInteractionInfo;
class UUDParameterEditorViewModel;

/**
 * Single interactable action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDTileInteractionViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/** Set content of the interaction option. */
	void SetContent(FIntPoint selectedTile, FUDTileInteractionInfo content);
public:
	// Button Functions
	/** Execution of the interaction. */
	UFUNCTION()
	void Interact();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText InteractText;
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
	// MVVM Setters & Getters
	void SetInteractText(FText newInteractText);
	FText GetInteractText() const;
	void SetParameterEditorContent(FUDViewModelContent newParameterEditorContent);
	FUDViewModelContent GetParameterEditorContent() const;
private:
	// Fields
	FIntPoint SelectedTile;
	FUDTileInteractionInfo Content;
	// Current Instance in use...
	FName ParameterEditorInstanceName = TEXT("TileParameterEditorInstance");
	TSubclassOf<UUDViewModel> ParameterEditorType;
	TObjectPtr<UUDParameterEditorViewModel> ParameterEditorInstance;
private:
	// Ensure uniqueness of all collections used...
	bool IsUniqueNameDefined = false;
	// Static trick to always have unique names for each instance.
	static int32 UniqueNameDefinition;
};