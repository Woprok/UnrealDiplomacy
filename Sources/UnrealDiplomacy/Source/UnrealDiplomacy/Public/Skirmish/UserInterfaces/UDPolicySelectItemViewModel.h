// Copyright Miroslav Valach

#pragma once

#include "CoreMinimal.h"
#include "Core/UserInterfaces/UDViewModel.h"
#include "Core/Simulation/UDModelStructs.h"
#include "UDPolicySelectItemViewModel.generated.h"

// Forward Declarations

struct FUDPolicySelectItemInfo;

/**
 * Single selectable item.
 */
UCLASS()
class UNREALDIPLOMACY_API UUDPolicySelectItemViewModel : public UUDViewModel
{
	GENERATED_BODY()
public:
	/**
	 * Set content of the interaction option.
	 */
	void SetContent(FUDPolicySelectItemInfo content);
public:
	// Button Functions
	/** Interacting with this item. */
	UFUNCTION()
	void Select();
	// MVVM Fields
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	FText SelectItemText;
	UPROPERTY(BlueprintReadWrite, FieldNotify, Setter, Getter)
	bool CanSelectItemValue;
public:
	virtual void Setup() override;
	virtual void Refresh() override;
private:
	// MVVM Setters & Getters
	void SetSelectItemText(FText newSelectItemText);
	FText GetSelectItemText() const;
	void SetCanSelectItemValue(bool newCanSelectItemValue);
	bool GetCanSelectItemValue() const;
private:
	// Fields
	FUDPolicySelectItemInfo Content;
};