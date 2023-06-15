// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "UDMessageItemViewModel.generated.h"

// Forward Declarations

/**
 * Single interactable item.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDMessageItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the interaction option.
	 */
	void SetContent();
public:
	// Button Functions
	/**
	 * Confirmation of the interaction.
	 */
	UFUNCTION()
	void Accept();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText AcceptText;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetAcceptText(FText newAcceptText);
	FText GetAcceptText() const;
private:
	// Fields
};