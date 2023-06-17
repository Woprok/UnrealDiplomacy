// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDTileInteractionViewModel.generated.h"

// Forward Declarations

struct FUDTileInteractionInfo;

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
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText InteractText;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetInteractText(FText newInteractText);
	FText GetInteractText() const;
private:
	// Fields
	FIntPoint SelectedTile;
	FUDTileInteractionInfo Content;
};