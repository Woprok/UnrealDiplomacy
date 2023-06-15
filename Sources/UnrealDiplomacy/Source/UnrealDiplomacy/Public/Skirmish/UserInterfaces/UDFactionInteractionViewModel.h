// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDFactionInteractionViewModel.generated.h"

// Forward Declarations

struct FUDFactionInteractionInfo;

/**
 * Single interactable action.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionInteractionViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
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
	FUDFactionInteractionInfo Content;
};