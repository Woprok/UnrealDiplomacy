// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDFactionInteractionViewModel.generated.h"

// Forward Declarations

struct FUDFactionInteractionInfo;
class UUDParameterEditorViewModel;

DECLARE_MULTICAST_DELEGATE_OneParam(FUDParameterEditorChanged, const TObjectPtr<UUDParameterEditorViewModel>& editorViewModel);

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
	 void SetContent(FUDFactionInteractionInfo content);
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
	/** Defines all parameter isntances. */
	void DefineInstances();
private:
	// MVVM Setters & Getters
	void SetInteractText(FText newInteractText);
	FText GetInteractText() const;
private:
	// Fields
	FUDFactionInteractionInfo Content;
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