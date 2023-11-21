// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDFactionItemViewModel.generated.h"

// Forward Declarations

struct FUDFactionInfo;

/**
 * Single faction in a list.
 */
UCLASS(Blueprintable, BlueprintType)
class UNREALDIPLOMACY_API UUDFactionItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the strategy option.
	 */
	void SetContent(FUDFactionInfo content);
public:
	// Button Functions
	/**
	 * Notifies the UI about nation being selected.
	 */
	UFUNCTION()
	void Interact();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText NameText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText ControllerText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool IsHumanValue;
public:
	virtual void Initialize() override;
	virtual void Update() override;
private:
	// MVVM Setters & Getters
	void SetNameText(FText newNameText);
	FText GetNameText() const;
	void SetControllerText(FText newControllerText);
	FText GetControllerText() const;
	void SetIsHumanValue(bool newIsHumanValue);
	bool GetIsHumanValue() const;
private:
	// Fields
	FUDFactionInfo Content;
};